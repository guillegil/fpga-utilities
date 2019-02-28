#include "clk_wiz.h"
#include <sys/mman.h>

void clkwiz_init(void *clk_wiz)
{
  static uint8_t n_wiz = 0;
  uint8_t pc;

  if(!n_wiz)
  {
      pthread_mutex_init(&clkwiz_addmutex, NULL);
      pthread_mutexattr_init(&clkwiz_addmutexattr);

      pthread_attr_init(&clkwiz.threadattr);

      clkwiz.data = (struct clkwiz_data **)malloc(sizeof(struct clkwiz_data *));

      clkwiz.active = 0;


      pthread_mutex_lock(&clkwiz_addmutex);
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
     clkwiz.clkwiz_map[n_wiz] = clk_wiz;

     clkwiz.data[clkwiz.active] = (struct clkwiz_data *)malloc(sizeof(struct clkwiz_data *));
     memset(clkwiz.data[clkwiz.active], 0, sizeof(struct clkwiz_data *));

     clkwiz.tasks = (clkwiz_flags *)malloc(sizeof(clkwiz_flags));
     memset(clkwiz.tasks, 0, sizeof(clkwiz_flags));


     ++clkwiz.active;

     pthread_mutex_init(&clkwiz.mutex[n_wiz], NULL);
     pthread_mutexattr_init(&clkwiz.mutexattr[n_wiz]);

     pthread_mutex_unlock(&clkwiz_addmutex);
     ++n_wiz;
  }else
  {
    perror("More MMCM/PLL is not suppoted by this device");
  }
}



void *clkwiz_thread(void *argg)
{
    uint8_t i, check;

    while(1)
    {
      pthread_mutex_lock(&clkwiz_addmutex);

      for(i = 0; i < clkwiz.active; ++i)
      {
        check = pthread_mutex_trylock(&clkwiz.mutex[i]);
        if(!check)
        {
          switch(clkwiz.tasks[i].all)
          {
            case 0x01:                // TODO: Create predefined values. Hexadecial ones
                                      //       are only for test.
              printf("Case 0x01:\n");
              break;
            case 0x02:
              printf("Case 0x02:\n");
              printf("Data from struct: %d\n", (*clkwiz.data)[i].reg_value);
              clkwiz.tasks[i].all = 0;
              break;
            case 0x04:
              printf("Case 0x04:\n");
              break;
          }
        }

        pthread_mutex_unlock(&clkwiz.mutex[i]);
      }


      pthread_mutex_unlock(&clkwiz_addmutex);
      //TODO: Add semaphore
    }

    pthread_exit(NULL);
}






/* Test Function */

void test_thread_com(void *clk_wiz, uint32_t multiply, uint8_t c)
{
  pthread_mutex_lock(&clkwiz_addmutex);

  uint8_t i = 0;
  while(i < clkwiz.active)
  {
    if(clk_wiz == clkwiz.clkwiz_map[i])
      break;

    ++i;
  }

  clkwiz.tasks[i].all = c;
  (*clkwiz.data)[i].reg_value = multiply;

  pthread_mutex_unlock(&clkwiz_addmutex);
}


