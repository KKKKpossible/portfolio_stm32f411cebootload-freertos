/*
 * ap_flash.c
 *
 *  Created on: 2023. 3. 7.
 *      Author: ksj10
 */


#include "ap_flash.h"


#define SECTOR_LENGTH (8U)


flash_t flash_inst[SECTOR_LENGTH];
uint32_t err_address;


static void apFlashWriteByte(uint32_t address, uint16_t data);
static bool apAddrInSectorCheck(uint32_t sector_num, uint32_t data_address, uint32_t data_length);
static bool apAddrInSecterRangeCheck(uint32_t sector_num, uint32_t data_address, uint32_t data_length);


void apFlashInit(void)
{
	for(int i = 0; i < SECTOR_LENGTH; i++)
	{
		switch(i)
		{
		    case 0:
		        flash_inst[i].start_address = 0x8000000;
                flash_inst[i].size = 0x4000;
		        break;
            case 1:
                flash_inst[i].start_address = 0x8004000;
                flash_inst[i].size = 0x4000;
                break;
            case 2:
                flash_inst[i].start_address = 0x8008000;
                flash_inst[i].size = 0x4000;
                break;
            case 3:
                flash_inst[i].start_address = 0x800C000;
                flash_inst[i].size = 0x4000;
                break;
            case 4:
                flash_inst[i].start_address = 0x8010000;
                flash_inst[i].size = 0x10000;
                break;
            case 5:
                flash_inst[i].start_address = 0x8020000;
                flash_inst[i].size = 0x20000;
                break;
            case 6:
                flash_inst[i].start_address = 0x8040000;
                flash_inst[i].size = 0x20000;
                break;
            case 7:
                flash_inst[i].start_address = 0x8060000;
                flash_inst[i].size = 0x20000;
                break;
            default:
                break;
		}
	}
}

void apFlashWriteData(uint32_t start_address, uint8_t *p_data, uint32_t length)
{
	uint32_t buff_start_address = start_address;

    HAL_FLASH_Unlock();
	for(int i = 0; i < length; i++)
	{
		apFlashWriteByte(buff_start_address, p_data[i]);
		buff_start_address += 1;
	}
    HAL_FLASH_Lock();
}

void apFlashErase(uint32_t start_address, uint32_t length)
{
    uint32_t err = 0;
    int start_sector = -1;
    int sector_number = 0;
    HAL_StatusTypeDef state;
    FLASH_EraseInitTypeDef erase_init;


    for(int i = FLASH_SECTOR_0; i < SECTOR_LENGTH; i++)
    {
        if(apAddrInSectorCheck(i, start_address, length) == true)
        {
            if(start_sector < 0)
            {
                start_sector = i;
            }
            sector_number += 1;
        }
    }
    erase_init.Banks = FLASH_BANK_1;
    erase_init.Sector = start_sector;
    erase_init.NbSectors = sector_number;
    erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    state = HAL_FLASHEx_Erase(&erase_init, &err);
    HAL_FLASH_Lock();

    if(state == HAL_ERROR)
    {
        err_address = err;
    }
}

uint8_t apFlashReadByte(uint32_t address)
{
	uint8_t ret = (uint8_t)(*(uint32_t *)address);

	return ret;
}

static void apFlashWriteByte(uint32_t address, uint16_t data)
{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, data);
}

static bool apAddrInSectorCheck(uint32_t sector_num, uint32_t data_address, uint32_t data_length)
{
    bool ret = false;

    uint32_t start_data_address = data_address;
    uint32_t stop_data_address = data_address + data_length - 1;
    uint32_t start_sector_address = flash_inst[sector_num].start_address;
    uint32_t stop_sector_address = flash_inst[sector_num].start_address + flash_inst[sector_num].size - 1;

    if(start_data_address >= start_sector_address && start_data_address <= stop_sector_address)
    {
        ret = true;
    }
    if(stop_data_address >= start_sector_address && stop_data_address <= stop_sector_address)
    {
        ret = true;
    }
    if(start_data_address <= start_sector_address && stop_data_address >= stop_sector_address)
    {
        ret = true;
    }

    return ret;
}

static bool apAddrInSecterRangeCheck(uint32_t sector_num, uint32_t data_address, uint32_t data_length)
{
    bool ret = false;

    uint32_t start_data_address = data_address;
    uint32_t stop_data_address = data_address + data_length - 1;
    uint32_t start_sector_address = flash_inst[sector_num].start_address;
    uint32_t stop_sector_address = flash_inst[sector_num].start_address + flash_inst[sector_num].size - 1;

    if(start_data_address >= start_sector_address && start_data_address <= stop_sector_address)
    {
        ret = true;
    }
    if(stop_data_address >= start_sector_address && stop_data_address <= stop_sector_address)
    {
        ret = true;
    }

    return ret;
}
