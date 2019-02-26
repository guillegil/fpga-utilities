#include "clk_wiz.h"

void clk_wiz_init(void *clk_wiz)
{
  static uint8_t n_wiz = 0;

  if(n_wiz < N_MMCM)
  {
    const char *queue_dir = "/MyQueue"; // FIXME: Change the name, also below
    struct mq_attr at;  // FIXME: Remove me
    clk_wiz_prop[n_wiz].clk_wiz = clk_wiz;
    clk_wiz_prop[n_wiz].id = n_wiz;

    pthread_attr_init(&clk_wiz_prop[n_wiz].clk_wiz_thread_attr);

    clk_wiz_prop[n_wiz].queue_attr.mq_msgsize = sizeof(uint32_t)*2;
    clk_wiz_prop[n_wiz].queue_attr.mq_maxmsg = 1;
    clk_wiz_prop[n_wiz].queue_attr.mq_flags = 0;
    clk_wiz_prop[n_wiz].queue_attr.mq_curmsgs = 0;

    //sprintf(queue_dir, "%s%d%s", "/home/guille/Documents/", clk_wiz_prop[n_wiz].id, "th");
    printf("Dir: %s\n", queue_dir);
    mq_unlink("/MiQueue");
    clk_wiz_prop[n_wiz].queue = mq_open("/MiQueue", (O_RDWR|O_CREAT), (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH), &(clk_wiz_prop[n_wiz].queue_attr));
    if(clk_wiz_prop[n_wiz].queue < 0)
    {
      perror("mq_open error");
      exit(-1);
    }



    uint8_t rc = pthread_create(&clk_wiz_prop[n_wiz].clk_wiz_thread, &clk_wiz_prop[n_wiz].clk_wiz_thread_attr, clk_wiz_write, &clk_wiz_prop[n_wiz]);
    if(rc)
    {
      printf("ERROR: pthread_create() is %d\n", rc);
      exit(-1);
    }
    ++n_wiz;
  }else
  {
    perror("More MMCM is not suppoted by this device");
  }
}


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

    *((uint32_t *)(clk_wiz + dir_offset)) = data;   // TODO: This can be managed by a thread

    update(clk_wiz);
}

void read_all_clk_reg(void *clk_wiz)
{
    uint8_t i;
    for(i = 0; i < CLK_WIZ_ALL_DATA_REG; ++i)
        printf("Register in offset %#05x = %#010x\n", i*0x4 + 0x200,*((uint32_t *)(clk_wiz + i*0x4 + 0x200)));
}


void *clk_wiz_write(void *argg)
{
    struct clock_wizard clk_wiz = *((struct clock_wizard *)(argg));
    struct clock_wizard_pkg clk_wiz_data;
    unsigned prio = 1;
    char *buff = (char *)malloc(sizeof(uint32_t)*2);
    uint8_t state = 0;

    while(1)
    {
        mq_receive(clk_wiz.queue, buff, 1000, &prio); // TODO: Change de length value
        printf("buff = %s\n\n", buff);

        switch(state) // TODO: Define states
        {
          case 0:
            if(buff[0] == 'o')
            {
              clk_wiz_data.dir_offset = atoi((buff + 1));
              state = 1;
            }
            break;
          case 1:
            if(buff[0] == 'd')
            {
              clk_wiz_data.data = atoi((buff + 1));
              state = 1;
            }
            break;
          case 2:
            // Write into registers maybe valide
            state = 0;
            break;

        }

        printf("Thread id: %d\nOffset: %d\nData: %d\n\n", clk_wiz.id, clk_wiz_data.dir_offset, clk_wiz_data.data);

        sleep(1);
    }

    printf("I'm the thread with id: %d\n", clk_wiz.id); // Only for debug
    pthread_exit(NULL);
}






/* Test Function */


void queue_send_test(void *clk_wiz, uint32_t offset, uint32_t data)
{
  struct clock_wizard_pkg pkg;
  unsigned prio = 0;
  pkg.dir_offset = offset;
  pkg.data = data;

  char *buff = (char *)malloc(sizeof(uint32_t)*2);


  uint8_t i = 0;
  while(i < N_MMCM)           // it looks for the Clock Wizard
  {
    if(clk_wiz == clk_wiz_prop[i].clk_wiz)
      break;

    ++i;
  }

  printf("i = %d\n\n", i);
  sprintf(buff, "%c%u", 'o', pkg.dir_offset);

  printf("I'm going to send %s\n", buff);

  if(mq_send(clk_wiz_prop[i].queue, buff, sizeof(uint32_t) + 1, prio) < 0)
  {
    perror("Error\n"); fflush(stdout);
  }

  sprintf(buff, "%c%u", 'd', pkg.data);
  printf("I'm going to send %s\n", buff);

  if(mq_send(clk_wiz_prop[i].queue, buff, sizeof(uint32_t) + 1, prio) < 0)
  {
    perror("Error\n"); fflush(stdout);
  }


}


