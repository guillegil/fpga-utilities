#ifndef __CLK_WIZ_H__
#define __CLK_WIZ_H__

#include <stdint.h>
#include <stdio.h>
#include "devicesmmcm.h"


#define N_MMCM          XC7Z010CLG400_1 


#define CLK_WIZ_ALL_DATA_REG     23
#define CLK_CONF_REG23           0x25C

#define CLK_WIZ_SR               0x04
#define CLK_WIZ_INT_STAT         0x0C
#define CLK_WIZ_INT_EN           0x10

//TODO: Complete all definitions for all registers


#define WAIT_FOR_LOCK(clk_wiz)  while(!(*((uint32_t *)(clk_wiz + CLK_WIZ_SR))))



void clk_wiz_init(void *clk_wiz);
void update(void *clk_wiz);     // This can be done easily 
void clk_wiz_write_reg(void *clk_wiz, uint32_t dir_offset, uint32_t data);
void read_all_clk_reg(void *clk_wiz);







#endif // __CLK_WIZ_H__