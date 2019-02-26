#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "clk_wiz.h"




#define CLK_WIZ0        0x43C00000
#define CLK_CONF_REG5   0x214


int main(int args, char **argv)
{

  //int fd;
  void *clk_wiz0, *clk_wiz1;
  //char *name = "/dev/mem";

  // if((fd = open(name, O_RDWR | O_SYNC)) < 0)
  // {
  //   perror("open");
  //   return 1;
  // }

  //   clk_wiz0 = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, CLK_WIZ0);

  clk_wiz_init(clk_wiz0);
  //clk_wiz_init(clk_wiz1);


  printf("Aqui termina el main\n");

  while(1)
  {
      queue_send_test(clk_wiz0, 0x0000000F, 0x0000000A);
      sleep(1);
  }


  // munmap(clk_wiz0, sysconf(_SC_PAGESIZE));


    return 0;
}