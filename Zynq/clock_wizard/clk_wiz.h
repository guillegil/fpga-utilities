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


#define N_MMCM          XC7Z010CLG400_1 
#define QUEUE_DIR       "/"

#define CLK_WIZ_ALL_DATA_REG     23
#define CLK_CONF_REG23           0x25C

#define CLK_WIZ_SR               0x04
#define CLK_WIZ_INT_STAT         0x0C 
#define CLK_WIZ_INT_EN           0x10


#define WAIT_FOR_LOCK(clk_wiz)  while(!(*((uint32_t *)(clk_wiz + CLK_WIZ_SR))))


//TODO: Complete all definitions for all registers


struct clock_wizard
{
    void *clk_wiz;
    uint8_t id;
    pthread_t clk_wiz_thread;
    pthread_attr_t clk_wiz_thread_attr;

    mqd_t queue;
    struct mq_attr queue_attr;
}; 

struct clock_wizard_pkg
{
    uint32_t dir_offset;
    uint32_t data;
};

static struct clock_wizard clk_wiz_prop[N_MMCM];






void clk_wiz_init(void *clk_wiz);

void update(void *clk_wiz);     // This can be done easily 
void clk_wiz_write_reg(void *clk_wiz, uint32_t dir_offset, uint32_t data);
void read_all_clk_reg(void *clk_wiz);

void *clk_wiz_write(void *argg);

void queue_send_test(void *clk_wiz, uint32_t offset, uint32_t data);


#endif // __CLK_WIZ_H__