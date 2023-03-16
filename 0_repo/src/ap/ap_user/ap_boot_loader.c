/*
 * ap_boot_loader.c
 *
 *  Created on: 2023. 3. 8.
 *      Author: ksj10
 */


#include "ap_boot_loader.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"


extern USBD_HandleTypeDef hUsbDeviceFS;


boot_t boot_inst;


static void apBootInitLoop(void);


void apBootInit(void)
{
    boot_inst.tag_version_data = 0x10000000;

    boot_inst.tag_address_start = 0x8010000;
    boot_inst.tag_address_end = boot_inst.tag_address_start + 0x10000 - 1;

    boot_inst.tag_firmware_address_start = 0x8010000;
    boot_inst.tag_firmware_address_end = 0x8010003;
    boot_inst.tag_version_address_start = 0x8010004;
    boot_inst.tag_version_address_end = 0x8010007;

    boot_inst.firmware_address_start = 0x8020000;
    boot_inst.firmware_address_end = boot_inst.firmware_address_start + 0x60000 - 1;

    apBootLoadTag();

    while(true)
    {
        apBootInitLoop();
        if(ap_sys_inst.jump_ready == true)
        {
            if(millis() - ap_sys_inst.tick_to_jump > 1000)
            {
                ap_sys_inst.jump_ready = false;

                 apBootJumpFirmware();
            }
        }
    }
}

void apBootSaveFirmware(void)
{
    apFlashWriteData(boot_inst.writing_address, boot_inst.file_writer, boot_inst.received_file_length);
    boot_inst.writing_address += boot_inst.received_file_length;
}

void apBootJumpFirmware(void)
{
    apBootBspDeInit();
    (*boot_inst.jumper)();
}

void apBootVertifyFirmware(void)
{

}

void apBootDeleteFirmware(void)
{
    uint32_t length = boot_inst.firmware_address_end - boot_inst.firmware_address_start;

    apFlashErase(boot_inst.firmware_address_start, length);
}

void apBootSaveTag(void)
{
    uint8_t writer[4] = {0, };
    uint8_t *p_data = (uint8_t *)&boot_inst.tag_firmware_address_data;
    int length = boot_inst.tag_firmware_address_end - boot_inst.tag_firmware_address_start + 1;

    boot_inst.tag_firmware_address_data = boot_inst.rx_start_address;
    // TODO: tag version receive transmit 필요한듯
    // boot_inst.tag_version_data = ?;

    for(int i = 0; i < length; i++)
    {
        writer[i] = p_data[i];
    }

    apFlashWriteData(boot_inst.tag_address_start, writer, 4);

    length = boot_inst.tag_version_address_end - boot_inst.tag_version_address_start + 1;
    p_data = (uint8_t *)&boot_inst.tag_version_data;

    for(int i = 0; i < length; i++)
    {
        writer[i] = p_data[i];
    }

    apFlashWriteData(boot_inst.tag_address_start, writer, 4);
}

void apBootLoadTag(void)
{
    bool jump = false;

    uint8_t *p_writer = (uint8_t *)&boot_inst.tag_firmware_address_data;
    int length = boot_inst.tag_firmware_address_end - boot_inst.tag_firmware_address_start + 1;

    for(int i = 0; i < length; i++)
    {
        p_writer[i] = apFlashReadByte(boot_inst.tag_firmware_address_start + i);
    }

    length = boot_inst.tag_version_address_end - boot_inst.tag_version_address_start + 1;
    p_writer = (uint8_t *)&boot_inst.tag_version_data;
    for(int i = 0; i < length; i++)
    {
        p_writer[i] = apFlashReadByte(boot_inst.tag_version_address_start + i);
    }

    if(jump == true)
    {
        (*boot_inst.jumper)();
    }
}

void apBootDeleteTag(void)
{
    uint32_t length = boot_inst.tag_address_end - boot_inst.tag_address_start + 1;

    apFlashErase(boot_inst.tag_address_start, length);
}

void apBootBspDeInit(void)
{
    USBD_DeInit(&hUsbDeviceFS);

    HAL_UART_Abort(&huart1);
    if (HAL_UART_DeInit(&huart1) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_SPI_MspDeInit(&hspi1);

    HAL_I2C_MspDeInit(&hi2c1);

    HAL_RCC_DeInit();

    for(int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        __DSB();
        __ISB();
    }
    SysTick->CTRL = 0;
}

static void apBootInitLoop(void)
{
    apParserFetchComm();
    apSysStateOperate();
}

