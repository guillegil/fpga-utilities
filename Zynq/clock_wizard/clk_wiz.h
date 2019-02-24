#ifndef __CLK_WIZ_H__
#define __CLK_WIZ_H__

#define UPDATE_CLK_WIZ(clk_wiz) {*(clk_wiz+UPDATE_REG) = 0x00000003; *(clk_wiz+UPDATE_REG) = 0x00000002; *(clk_wiz) = 0x0000000A}


/*
*   This function maps the memory dir where is the clock_wizard located.
*/

void clk_wiz_init(void *clk_wiz, uint32_t dir);

void clk_wiz_write_reg(uint32_t data, uint32_t dir_offset);






#endif // __CLK_WIZ_H__