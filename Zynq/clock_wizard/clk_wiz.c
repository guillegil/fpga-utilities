#include "clk_wiz.h"
#include <sys/mman.h>

void clkwiz_init(void *clk_wiz)
{
  static uint8_t n_wiz = 0;
  uint8_t pc;

  if(!n_wiz)
  {
      pthread_mutex_init(&clkwiz_mutex, NULL);
      pthread_mutexattr_init(&clkwiz_mutexattr);

      pthread_attr_init(&clkwiz.threadattr);

      clkwiz.data = (struct clkwiz_data **)malloc(sizeof(struct clkwiz_data *));

      clkwiz.active = 0;
      pc = pthread_create(&clkwiz.thread, &clkwiz.threadattr,
           clkwiz_thread, NULL);
      if(pc)
      {
        perror("Error creating thread for Clock Wizard"); // TODO: Change for error()
        exit(-1);
      }
  }

  if(n_wiz < MMCM_PLL)
  {
     pthread_mutex_lock(&clkwiz_mutex);
     clkwiz.clkwiz_map[n_wiz] = clk_wiz;

     clkwiz.data[clkwiz.active] = (struct clkwiz_data *)malloc(sizeof(struct clkwiz_data *));
     memset(clkwiz.data[clkwiz.active], 0, sizeof(struct clkwiz_data *));

     clkwiz.tasks = (clkwiz_flags *)malloc(sizeof(clkwiz_flags));
     memset(clkwiz.tasks, 0, sizeof(clkwiz_flags));


     ++clkwiz.active;

     pthread_mutex_unlock(&clkwiz_mutex);
     ++n_wiz;
  }else
  {
    perror("More MMCM/PLL is not suppoted by this device");
  }
}



void *clkwiz_thread(void *argg)
{
    uint8_t i, mlock, pend = 0;

    while(1)
    {
      mlock = pthread_mutex_trylock(&clkwiz_mutex);
      if(!mlock)  // If not do other thing or block when sem_wait();
      {
        for(i = 0; i < clkwiz.active; ++i)
        {
          switch(clkwiz.tasks[i].all)
          {
            case CLKWIZ_RESET:
              break;
            case CLKWIZ_READ:
              (*clkwiz.data)[i].reg_value = *((uint32_t *)(clkwiz.clkwiz_map[i] +
                                            (*clkwiz.data)[i].outputs));
              clkwiz.tasks[i].all = 0;
              break;
            case CLKWIZ_WRITE:
              *((uint32_t *)(clkwiz.clkwiz_map[i] + (*clkwiz.data)[i].outputs)) = (*clkwiz.data)[i].reg_value;

              SET_CLK_UPDATE(clkwiz.clkwiz_map[i]);

              if(*((uint32_t *)(clkwiz.clkwiz_map[i] + (*clkwiz.data)[i].outputs)) != (*clkwiz.data)[i].reg_value) // TODO: Do not block. DONE!
              {
                pend = 1;
              }else
              {
                UNSET_CLK_UPDATE(clkwiz.clkwiz_map[i]);
                clkwiz.tasks[i].all = 0;
                pend = 0;
              }


              break;
          }
        }
      }

      // if pend = 1 do not sleep.
      pthread_mutex_unlock(&clkwiz_mutex);
      //TODO: Add semaphore
    }

    pthread_exit(NULL);
}






/* Test Function */

void test_thread_com(void *clk_wiz, uint32_t multiply, uint8_t c)
{
  pthread_mutex_lock(&clkwiz_mutex);

  uint8_t i = 0;
  while(i < clkwiz.active)
  {
    if(clk_wiz == clkwiz.clkwiz_map[i])
      break;

    ++i;
  }

  clkwiz.tasks[i].all = c;
  (*clkwiz.data)[i].reg_value = multiply;

  pthread_mutex_unlock(&clkwiz_mutex);
}


int clk_divide(void *clk_wiz, uint8_t div, uint8_t output)
{
  uint8_t mcheck = 0;
  pthread_mutex_lock(&clkwiz_mutex);
  if(!mcheck)
  {
    uint8_t i = 0;
    while(i < clkwiz.active)
    {
      if(clk_wiz == clkwiz.clkwiz_map[i])
      break;

      ++i;
    }

    // If i == clkwiz.active do something.

    switch(output)
    {
      case CLK_OUTPUT_ALL:
        (*clkwiz.data)[i].outputs = CLK_CONF_REG0;
        if(div != 0)
        {
          (*clkwiz.data)[i].reg_value = (*((uint32_t *)(clkwiz.clkwiz_map[i] + CLK_CONF_REG0)) & 0xFFFFFF00) | ((uint32_t)(div << 0));

          clkwiz.tasks[i].f2 = 1;
        }else
        {
          return -1;
        }
        break;
      default:
        return -1;
        break;
    }

  }

  pthread_mutex_unlock(&clkwiz_mutex);

  return 0;
}
