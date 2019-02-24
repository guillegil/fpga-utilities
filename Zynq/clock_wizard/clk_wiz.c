#include "clk_wiz.h"















void clk_wiz_write_reg(uint32_t *clk_wiz, uint32_t dir_offset, uint32_t data)
{
    uint8_t i = 0;

    



    // 0x200

    for(i = 0; i < 10 - 1; ++i)
    {
        if(dir_offset == (0x200 +(i*0x4)))
            *(clk_wiz + (0x200 +(i*0x4))) = data;
        else
            *(clk_wiz + (0x200 +(i*0x4))) = *(clk_wiz + (0x200 +(i*0x4)));
    }

    UPDATE_CLK_WIZ(clk_wiz);

}



