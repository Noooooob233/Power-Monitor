#include <flash.h>

// #define PAGE0_ADDRESS ((uint32_t)0x800F800) // page62
// #define PAGE1_ADDRESS ((uint32_t)0x800FC00) // page63

#define PAGE0_ADDRESS ((uint32_t)0x8010400)
#define PAGE1_ADDRESS ((uint32_t)0x8010800)

#define PAGE_ERASED ((uint16_t)0xFFFF)
#define PAGE_TRAINING ((uint16_t)0x1234)
#define PAGE_VALID ((uint16_t)0x0000)

void eeprom_copyValidVal();
void eeprom_format();
uint32_t eeprom_getValidPage();
uint16_t eeprom_findEmptyIdx(uint32_t page_start);

uint16_t valIdx = 0; // 指示当前空位的位置

#define VALNUM 2
uint16_t valTable[VALNUM] = {0x1000, 0x2000}; // 变量虚拟地址

/**
 * @description: 模拟eeprom初始化
 */
void eeprom_init()
{
    uint16_t page0_states = *((uint16_t *)PAGE0_ADDRESS);
    uint16_t page1_states = *((uint16_t *)PAGE1_ADDRESS);

    if (page0_states == PAGE_VALID && page1_states == PAGE_ERASED)
    {
        flash_erase(PAGE1_ADDRESS, 1);
    }
    else if (page0_states == PAGE_ERASED && page1_states == PAGE_VALID)
    {
        flash_erase(PAGE0_ADDRESS, 1);
    }
    else if (page0_states == PAGE_ERASED && page1_states == PAGE_TRAINING) // 上次传送被中断
    {
        uint16_t states = PAGE_VALID;
        flash_erase(PAGE0_ADDRESS, 1);
        flash_write(PAGE1_ADDRESS, &states, 1);
    }
    else if (page0_states == PAGE_TRAINING && page1_states == PAGE_ERASED)
    {
        uint16_t states = PAGE_VALID;
        flash_erase(PAGE1_ADDRESS, 1);
        flash_write(PAGE0_ADDRESS, &states, 1);
    }
    else if ((page0_states == PAGE_VALID && page1_states == PAGE_TRAINING) || (page0_states == PAGE_TRAINING && page1_states == PAGE_VALID))
    {
        eeprom_copyValidVal();
    }
    else
    {
        eeprom_format();
    }

    uint32_t validPage = eeprom_getValidPage();
    valIdx = eeprom_findEmptyIdx(validPage);
    if (valIdx == 0) // 若本页满了 提取有效数据到新页
    {
        eeprom_copyValidVal();
    }
}

/**
 * @description: 格式化flash
 */
void eeprom_format()
{
    uint16_t val = PAGE_VALID;
    flash_erase(PAGE0_ADDRESS, 1);
    flash_erase(PAGE1_ADDRESS, 1);
    flash_write(PAGE0_ADDRESS, &val, 1);
}

/**
 * @description: 读取一个数据
 * @param vaddress 数据的虚拟地址 
 * @param data 数据
 * @return 读取成功返回0 失败返回1
 */
uint8_t eeprom_read(uint16_t vaddress, uint16_t *data)
{
    uint32_t page_start = eeprom_getValidPage();

    for (uint32_t addr = page_start + 1020; addr >= page_start + 4; addr -= 4)
    {
        if (*((uint16_t *)addr) == vaddress)
        {
            *data = *((uint16_t *)addr + 1);
            return 0;
        }
    }

    return 1;
}

/**
 * @description: 写一个数据
 * @param vaddress 数据的虚拟地址 
 * @param data 数据
 */
void eeprom_write(uint16_t vaddress, uint16_t data)
{
    uint32_t page_start = eeprom_getValidPage();
    uint32_t page_end = page_start + 1023;
    uint16_t val[2] = {vaddress, data};

    if (page_start == 0)
    {
        return;
    }

    flash_write(page_start + (valIdx << 2), val, 2);
    valIdx++;

    if (page_start + (valIdx << 2) >= page_end) // 若本页满了 提取有效数据到新页
    {
        eeprom_copyValidVal();
    }
}

/**
 * @description: 将有效数据复制到另一页
 */
void eeprom_copyValidVal()
{
    uint32_t old_page = PAGE0_ADDRESS;
    uint32_t new_page = PAGE1_ADDRESS;

    if (*((uint16_t *)PAGE0_ADDRESS) == PAGE_VALID)
    {
        old_page = PAGE0_ADDRESS;
        new_page = PAGE1_ADDRESS;
    }
    else if (*((uint16_t *)PAGE1_ADDRESS) == PAGE_VALID)
    {
        old_page = PAGE1_ADDRESS;
        new_page = PAGE0_ADDRESS;
    }

    uint16_t states = PAGE_TRAINING;
    uint16_t val[2] = {0, 0};

    valIdx = eeprom_findEmptyIdx(new_page);
    flash_write(new_page, &states, 1);
    for (uint16_t i = 0; i < VALNUM; i++)
    {
        if (eeprom_read(valTable[i], val + 1) == 0)
        {
            val[0] = valTable[i];
            flash_write(new_page + (valIdx << 2), val, 2);
            valIdx++;
        }
    }
    
    flash_erase(old_page, 1);
    states = PAGE_VALID;
    flash_write(new_page, &states, 1);
}

/**
 * @description: 寻找页中第一个空位的索引
 * @param page_start 页的起始地址 
 * @return 读取成功返回索引值 失败返回0
 */
uint16_t eeprom_findEmptyIdx(uint32_t page_start)
{
    uint32_t page_end = page_start + 1023;
    uint16_t ret = 1;

    for (uint32_t addr = page_start + 4; addr < page_end; addr += 4) // 跳过头部4个字节
    {
        if (*(uint32_t *)addr == 0xFFFFFFFF)
        {
            return ret;
        }
        ret++;
    }

    return 0;
}

uint32_t eeprom_findEmptyAddress(uint32_t page_start)
{
    uint32_t page_end = page_start + 1023;

    for (uint32_t addr = page_start + 4; addr < page_end; addr += 4) // 跳过头部4个字节
    {
        if (*(uint32_t *)addr == 0xFFFFFFFF)
        {
            return addr;
        }
    }

    return 0;
}

/**
 * @description: 寻找有效页
 * @return 读取成功返回有效页起始地址 失败返回0
 */
uint32_t eeprom_getValidPage()
{
    uint16_t page0_states = *((uint16_t *)PAGE0_ADDRESS);
    uint16_t page1_states = *((uint16_t *)PAGE1_ADDRESS);

    if (page0_states == PAGE_VALID && page1_states != PAGE_VALID)
    {
        return PAGE0_ADDRESS;
    }
    else if (page0_states != PAGE_VALID && page1_states == PAGE_VALID)
    {
        return PAGE1_ADDRESS;
    }

    return 0;
}

/**
 * @description: flash清除
 * @param page_start 页的起始地址 
 * @param num 页数
 * @return 失败返回擦除出错地址值
 */
uint32_t flash_erase(uint32_t address, uint32_t num)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = address; //页地址
    EraseInitStruct.NbPages = num;         // (Byte / N) + ((Byte % N) ? 1 : 0);                       //待擦除的页数

    uint32_t PageError = 0;

    HAL_FLASH_Unlock();

    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

    HAL_FLASH_Lock();

    return PageError;
}

/**
 * @description: 往flash中写入值
 * @param page_start 地址 
 * @param num 写入数据数组
 * @param num 数组长度
 */
void flash_write(uint32_t address, uint16_t *p, int len)
{
    HAL_FLASH_Unlock();

    for (uint16_t i = 0; i < len; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, (uint64_t)*p);

        address += 2;
        p++;
    }

    HAL_FLASH_Lock();
}

/**
 * @description: 从flash中读出值
 * @param page_start 地址 
 * @param num 读取数据数组
 * @param num 数组长度
 */
void flash_read(uint32_t address, uint16_t *p, int len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        *p = *(uint16_t *)address;
        address += 2;
        p++;
    }
}
