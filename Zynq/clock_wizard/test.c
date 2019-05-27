#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mmcm_controll.h"

#define CLK_WIZ0        0x43C00000


void read_all_cfg(void *cfg)
{
    for(int i = 0; i < 20; ++i)
    {
        printf("Reg +%#05x = %#010x\n", 0x200 + 4*i, *((uint32_t *)(cfg + 0x200 + 4*i)));
    }
}

int main(int args, char **argv)
{

  int fd;
  char *name = "/dev/mem";  

  mmcm_prop_t clk1_prop;
  clk1_prop.fin = 125000000.000f;
  clk1_prop.addr = CLK_WIZ0;
  clk1_prop.active_outputs = 2;
  clk1_prop.primitive = PLL_PRIMITIVE;
  clk1_prop.features.all = 0xFFFF;

  if((fd = open(name, O_RDWR)) < 0)
  {
    perror("open");
    return 1;
  }


  // void *cfg = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE,  MAP_SHARED, fd, CLK_WIZ0);
  // read_all_cfg(cfg);


  int clk_id;
  clk_id = init_clock_device(fd, &clk1_prop) ;
  if(clk_id < 0)
  {
    perror("Init clock device");
    exit(1);
  }

  sleep(2);

  printf("Output 0 frequency %f MHz\n", get_frequency(clk_id, 0)/1000000.0f);
  printf("Output 0 frequency %f MHz\n", set_frequency(clk_id, 0, 200000000.000f)/1000000.0f);

  sleep(2);

  printf("Output 0 frequency %f MHz\n", set_frequency(clk_id, 0, 300000000.000f)/1000000.0f);

  sleep(2);

  printf("Output 0 frequency %f MHz\n", set_frequency(clk_id, 0, 200000000.000f)/1000000.0f);

  sleep(2);

  printf("Output 0 frequency %f MHz\n", set_frequency(clk_id, 0, 100000000.000f)/1000000.0f);

  sleep(2);

  printf("Output 0 frequency %f MHz\n", set_frequency(clk_id, 0, 50000000.000f)/1000000.0f);

  close_clock_device(clk_id);
  return 0;
}
