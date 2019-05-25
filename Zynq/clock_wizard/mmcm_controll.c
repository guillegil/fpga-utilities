#include "mmcm_controll.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


float get_frac(uint32_t val)
{
    float value = (float)val;
    while(value > 1.0f)
        value = value/10.0f;

    return value;
}

uint32_t get_reg_value(void *cfg, uint32_t offset, uint8_t bup, uint8_t bdown)
{
    uint32_t reg_val = *((uint32_t *)(cfg + offset));
    uint32_t mask;

    switch(bup - bdown)
    {
        default:
            mask = 0xFFFFFFFF;
            break;
        case 1:
            mask = 0x00000001;
            break;
        case 2:
            mask = 0x00000003;
            break;
        case 3:
            mask = 0x00000007;
            break;
        case 4:
            mask = 0x0000000F;
            break;
        case 5:
            mask = 0x0000001F;
            break;
        case 6:
            mask = 0x0000003F;
            break;
        case 7:
            mask = 0x0000007F;
            break;
        case 8:
            mask = 0x000000FF;
            break;
        case 9:
            mask = 0x000001FF;
            break;
        case 10:
            mask = 0x000003FF;
            break;
        case 11:
            mask = 0x000007FF;
            break;
        case 12:
            mask = 0x00000FFF;
            break;
        case 13:
            mask = 0x00001FFF;
            break;
        case 14:
            mask = 0x00003FFF;
            break;
        case 15:
            mask = 0x00007FFF;
            break;
        case 16:
            mask = 0x0000FFFF;
            break;
        case 17:
            mask = 0x0001FFFF;
            break;
        case 18:
            mask = 0x0003FFFF;
            break;
        case 19:
            mask = 0x0007FFFF;
            break;
        case 20:
            mask = 0x000FFFFF;
            break;
        case 21:
            mask = 0x001FFFFF;
            break;
        case 22:
            mask = 0x003FFFFF;
            break;
        case 23:
            mask = 0x007FFFFF;
            break;
        case 24:
            mask = 0x00FFFFFF;
            break;
        case 25:
            mask = 0x01FFFFFF;
            break;
        case 26:
            mask = 0x03FFFFFF;
            break;
        case 27:
            mask = 0x07FFFFFF;
            break;
        case 28:
            mask = 0x0FFFFFFF;
            break;
        case 29:
            mask = 0x1FFFFFFF;
            break;
        case 30:
            mask = 0x3FFFFFFF;
            break;
        case 31:
            mask = 0x7FFFFFFF;
            break;
        case 32:
            mask = 0xFFFFFFFF;
            break;
    }
    
    return ((reg_val >> bdown) & mask);
}
void read_all(int id)
{
    for(int i = 0; i < 20; ++i)
    {
        printf("Reg +%#05x = %#010x\n", CLK_CONF_REG0 + 4*i, __read_reg(__mmcm_devices[id].cfg, CLK_CONF_REG0 + 4*i));
    }
}



/*
*
* Function name: init_clock_device();
* Parameters:
*   int fd:  descriptor where are located all the the devices, it's usually "/dev/map"
*   const mmcm_prop *mmcm_info: structure with information about the clock device either PLL or MMCM
*
* Description:
*     This function init a clock device. This function must be called before carry out any other operation 
*      over the clock device .
*
* Returns:    
*    On success returns the id of the clock device, that has to be used to carry out others operations over 
*    the clock device. On failure returns a negative number. If the clock device has been already initialize
*    it returns 0.
*
* */
int init_clock_device(int fd, const mmcm_prop_t *mmcm_info)
{
    if(__active_devices ==  MMCM_RESOURCES)
        return -1;
    

    for(int i = 0; i < __active_devices; ++i)
    {
        if(__mmcm_devices[i].info.addr == mmcm_info -> addr)
            return 0;
    }

    __mmcm_devices[__active_devices].cfg = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE,  MAP_SHARED, fd, mmcm_info -> addr);
    __mmcm_devices[__active_devices].info = *mmcm_info;
    __mmcm_devices[__active_devices].is_active = true;
    __mmcm_devices[__active_devices].ideal_m = (ceil((mmcm_info -> fin)/FPDF_MAX)*FVCO_MAX)/(mmcm_info -> fin);
    __mmcm_devices[__active_devices].d = get_reg_value(__mmcm_devices[__active_devices].cfg, CLK_CONF_REG0, 7, 0);
    __mmcm_devices[__active_devices].m = get_reg_value(__mmcm_devices[__active_devices].cfg, CLK_CONF_REG0, 15, 8);

    __mmcm_devices[__active_devices].m += get_frac(get_reg_value(__mmcm_devices[__active_devices].cfg, CLK_CONF_REG0, 25, 16));
    __mmcm_devices[__active_devices].id = __active_devices;
    __active_devices++;

    return (__active_devices - 1);
}

void close_clock_device(int id)
{
    if(id < __active_devices)
    {
        munmap(__mmcm_devices[id].cfg, sysconf(_SC_PAGESIZE));
        __active_devices--;
    }
}



float get_frequency(int id, uint8_t output)
{
    if(id > __active_devices)
        return -1;
    
    float o = get_reg_value(__mmcm_devices[id].cfg, CLK_CONF_REG2 + 0xC*output, 7, 0);


    if(output == 0)
    {
        o += get_frac(get_reg_value(__mmcm_devices[id].cfg, CLK_CONF_REG2, 17, 8));
        return (__mmcm_devices[id].m * __mmcm_devices[id].info.fin) /(__mmcm_devices[id].d * o);
    }

    return (__mmcm_devices[id].m * __mmcm_devices[id].info.fin)/(__mmcm_devices[id].d * o);
}

float set_frequency(int id, uint8_t output, uint32_t freq)
{
    if(id > __active_devices)
        return -1;

    uint32_t o = (__mmcm_devices[id].info.fin * __mmcm_devices[id].m)/((float)freq * __mmcm_devices[id].d); 

    if(o > 255)
        return -2;
    
    *((uint32_t *)(__mmcm_devices[id].cfg + CLK_CONF_REG2 + 0x0C * output)) = o;
    __program_clock_device(__mmcm_devices[id].cfg);


    return get_frequency(id, output);

}