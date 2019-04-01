#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "clk_wiz.h"

#define CLK_WIZ0        0x43C00000
#define NOUTPUTS        1

int main(int args, char **argv)
{

  int fd;
  void *clk_wiz0, *clk_wiz1;
  char *name = "/dev/mem";

  uint8_t all_outs = (CLK_OUTPUT1);

  uint8_t divs[] =
  {
    0x7f,
    0x7f
  };

  if((fd = open(name, O_RDWR | O_SYNC)) < 0)
  {
    perror("open");
    return 1;
  }

  clk_wiz0 = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE,  MAP_SHARED, fd, CLK_WIZ0);
  clkwiz_init(clk_wiz0, NOUTPUTS);

  read_all_clk_reg(clk_wiz0);
  clk_divide(clk_wiz0, divs, all_outs, 1);
  clk_divide_all(clk_wiz0, 0x7f);

  while(1)
  {
      sleep(5);
      read_all_clk_reg(clk_wiz0);
      break;
  }

  clk_terminate();
  close(fd);
  munmap(clk_wiz0, sysconf(_SC_PAGESIZE));


    return 0;
}
