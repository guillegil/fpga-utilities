#ifndef __CLK_WIZ_H__
#define __CLK_WIZ_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "devicesmmcm.h"
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#define MMCM_PLL                  XC7Z010CLG400_1



//TODO: Complete all definitions for all registers

/* Clock wizard AXI Lite registers */

#define CLK_SRR                  0x00   // Software Reset Register                  (W)
#define CLK_SR                   0x04   // Status Register                          (R)
#define CLK_MONITOR_SR           0x08   // Clock Monitor Error Status Register      (R)
#define CLK_IS                   0x0C   // Interrupt Status                         (R/W)
#define CLK_IEN                  0x10   // Interrupt Enable                         (R/W)

#define CLK_CONF_REG0            0x200  // Clock Configuration Register 0           (R/W)
#define CLK_CONF_REG1            0x204  // Clock Configuration Register 1           (R/W)
#define CLK_CONF_REG2            0x208  // Clock Configuration Register 2           (R/W)
#define CLK_CONF_REG3            0x20C  // Clock Configuration Register 3           (R/W)
#define CLK_CONF_REG4            0x210  // Clock Configuration Register 4           (R/W)
#define CLK_CONF_REG5            0x214  // Clock Configuration Register 5           (R/W)
#define CLK_CONF_REG6            0x218  // Clock Configuration Register 6           (R/W)
#define CLK_CONF_REG7            0x21C  // Clock Configuration Register 7           (R/W)
#define CLK_CONF_REG8            0x220  // Clock Configuration Register 8           (R/W)
#define CLK_CONF_REG9            0x224  // Clock Configuration Register 9           (R/W)
#define CLK_CONF_REG10           0x228  // Clock Configuration Register 10          (R/W)
#define CLK_CONF_REG11           0x22C  // Clock Configuration Register 11          (R/W)
#define CLK_CONF_REG12           0x230  // Clock Configuration Register 12          (R/W)
#define CLK_CONF_REG13           0x234  // Clock Configuration Register 13          (R/W)
#define CLK_CONF_REG14           0x238  // Clock Configuration Register 14          (R/W)
#define CLK_CONF_REG15           0x23C  // Clock Configuration Register 15          (R/W)
#define CLK_CONF_REG16           0x240  // Clock Configuration Register 16          (R/W)
#define CLK_CONF_REG17           0x244  // Clock Configuration Register 17          (R/W)
#define CLK_CONF_REG18           0x248  // Clock Configuration Register 18          (R/W)
#define CLK_CONF_REG19           0x24C  // Clock Configuration Register 19          (R/W)
#define CLK_CONF_REG20           0x250  // Clock Configuration Register 20          (R/W)
#define CLK_CONF_REG21           0x254  // Clock Configuration Register 21          (R/W)
#define CLK_CONF_REG22           0x258  // Clock Configuration Register 22          (R/W)
#define CLK_CONF_REG23           0x25C  // Clock Configuration Register 23          (R/W)

#define CLK_OUTPUT_ALL			(0)
#define CLK_OUTPUT1			 	(1)
#define CLK_OUTPUT2			 	(2)
#define CLK_OUTPUT3			 	(3)
#define CLK_OUTPUT4			 	(4)
#define CLK_OUTPUT5			 	(5)
#define CLK_OUTPUT6			 	(6)
#define CLK_OUTPUT7			 	(7)
#define CLK_OUTPUT8			 	(8)

#define CLKWIZ_RESET			 (1)
#define CLKWIZ_READ			     (2)
#define CLKWIZ_WRITE			 (4)

#define WAIT_FOR_LOCK(clk_wiz)  while(!(*((uint32_t *)(clk_wiz + CLK_WIZ_SR))))  // TODO: Remove when solved

#define SET_CLK_UPDATE(clk_wiz) *((uint32_t *)(clk_wiz + CLK_CONF_REG23)) = 0x00000003
#define UNSET_CLK_UPDATE(clk_wiz) *((uint32_t *)(clk_wiz + CLK_CONF_REG23)) = 0x00000002


typedef union {					// Flags
	unsigned char all;			// Todos

	struct {
		unsigned char f0 :1;		// Flag 0
		unsigned char f1 :1;		// Flag 1
		unsigned char f2 :1;		// Flag 2
		unsigned char f3 :1;		// Flag 3
		unsigned char f4 :1;		// Flag 4
		unsigned char f5 :1;		// Flag 5
		unsigned char f6 :1;		// Flag 6
		unsigned char f7 :1;		// Flag 7
	};

} clkwiz_flags;

struct clkwiz_data
{
    uint32_t  outputs;
    uint32_t reg_value;
};

pthread_mutex_t      clkwiz_mutex;
pthread_mutexattr_t  clkwiz_mutexattr;

struct clock_wizard_threadinfo
{
    pthread_t            thread;
    pthread_attr_t       threadattr;

    void                *clkwiz_map[MMCM_PLL];
    uint8_t              active;
    clkwiz_flags        *tasks;
    struct clkwiz_data **data;
};


struct clock_wizard_threadinfo clkwiz;


void clkwiz_init(void *clk_wiz);
void read_all_clk_reg(void *clk_wiz);

/* Threads */


void *clkwiz_thread(void *argg);


/* Test functions */

void test_thread_com(void *clk_wiz, uint32_t multiply, uint8_t c);
int clk_divide(void *clk_wiz, uint8_t div, uint8_t output);



#endif // __CLK_WIZ_H__