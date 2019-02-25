#include "clk_wiz.h"


void update(void *clk_wiz)
{
  *((uint32_t *)clk_wiz+CLK_CONF_REG23) = 0x00000003; 
  // TODO: Add delays.
  *((uint32_t *)clk_wiz+CLK_CONF_REG23) = 0x00000002; 
  *((uint32_t *)clk_wiz) = 0x0000000A;
}


void clk_wiz_write_reg(void *clk_wiz, uint32_t dir_offset, uint32_t data)
{

    printf("SR = %d\n", (*((uint32_t *)clk_wiz+CLK_WIZ_SR))); // Only for debug

    WAIT_FOR_LOCK(clk_wiz);                  // TODO: This function can not be blocked

    *((uint32_t *)(clk_wiz + dir_offset));   // TODO: This can be managed by a thread     

    update(clk_wiz);
}

void read_all_clk_reg(void *clk_wiz)
{
    uint8_t i;
    for(i = 0; i < CLK_WIZ_ALL_DATA_REG; ++i)
        printf("Register in offset %#05 = %#010\n", *((uint32_t *)(clk_wiz + i*0x4 + 0x200)));
    
}




