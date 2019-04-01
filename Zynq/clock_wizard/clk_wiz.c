#include "clk_wiz.h"
#include <sys/mman.h>

void clkwiz_init(void *clk_wiz, uint8_t n_outputs)
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

     printf("HERE!\n");

     clkwiz.data[clkwiz.active] = (struct clkwiz_data *)malloc(sizeof(struct clkwiz_data *));
     memset(clkwiz.data[clkwiz.active], 0, sizeof(struct clkwiz_data *));

     (*clkwiz.data)[clkwiz.active].reg_value = (uint32_t *)calloc(n_outputs, sizeof(uint32_t));

     memset((*clkwiz.data)[clkwiz.active].reg_value, 0, sizeof((*clkwiz.data)[clkwiz.active].reg_value));

     clkwiz.tasks = (clkwiz_flags *)malloc(sizeof(clkwiz_flags));
     memset(clkwiz.tasks, 0, sizeof(clkwiz_flags));

     clkwiz.n_outputs  = (uint8_t **)malloc(sizeof(uint8_t *));
     clkwiz.n_outputs[clkwiz.active]  = (uint8_t *)malloc(sizeof(uint8_t));
     (*clkwiz.n_outputs)[clkwiz.active] = n_outputs;

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
    uint8_t i, task_counter = 0b00000001, mlock;
    while(1)
    {
      mlock = pthread_mutex_trylock(&clkwiz_mutex);
      if(!mlock)
      {
        for(i = 0; i < clkwiz.active; ++i)
        {
          switch((task_counter & clkwiz.tasks[i].all)*(CHECK_LOCK(clkwiz.clkwiz_map[i])))
          {
            case CLKWIZ_RESET:
              *((uint32_t *)(clkwiz.clkwiz_map[i])) = 0xA;
              clkwiz.tasks[i].f0 = 0;
              break;
            case CLKWIZ_DIVIDE:
                SET_CLK_UPDATE(clkwiz.clkwiz_map[i]);
                if((*clkwiz.data)[i].outputs.all != 0)
                {
                  for(uint8_t j = 0; j < (*clkwiz.n_outputs)[i]; ++j)
                  {
                    if(((GET_OUTPUTS(i) >> j) & 0b00000001) && GET_REG_VALUE(i,CLK_CONF_REG2 + j*0xC) == GET_DATA(i,j))
                      (*clkwiz.data)[i].outputs.all = (*clkwiz.data)[i].outputs.all & (~(0b00000001 << j));
                  }
                }else
                {
                  UNSET_CLK_UPDATE(clkwiz.clkwiz_map[i]);
                  clkwiz.tasks[i].f1 = 0;
                }
              break;
            case CLKWIZ_DIVIDE_ALL:
              SET_CLK_UPDATE(clkwiz.clkwiz_map[i]);
              if(GET_REG_VALUE(i, CLK_CONF_REG0) == (*clkwiz.data)[i].reg_value[0])
              {
                clkwiz.tasks[i].f2 = 0;
                UNSET_CLK_UPDATE(clkwiz.clkwiz_map[i]);
              }
              break;
          }

          (task_counter == 0b10000000) ? (task_counter = 1) : (task_counter <<= 1);
        }
      }

      // if pend = 1 do not sleep.
      pthread_mutex_unlock(&clkwiz_mutex);
      //TODO: Add semaphore
    }

    pthread_exit(NULL);
}


void clk_divide(void *clk_wiz, uint8_t *div, uint8_t outputs, uint8_t this_update)
{
  pthread_mutex_lock(&clkwiz_mutex);
  uint8_t i = 0;
  while(i < clkwiz.active)
  {
    if(clk_wiz == clkwiz.clkwiz_map[i])
      break;

    ++i;
  }

  uint8_t j = 0, k = 0;
  (*clkwiz.data)[i].outputs.all = outputs;

  while(j < (*clkwiz.n_outputs)[i])
  {
     if((outputs >> j) & 0b00000001)
     {
        *((uint32_t *)(clkwiz.clkwiz_map[i] + CLK_CONF_REG2+j*0xC)) = div[k];
        (*clkwiz.data)[i].reg_value[j] = div[k];
        ++k;
       }
    ++j;
  }

  clkwiz.tasks[i].all |= CLKWIZ_DIVIDE;
  clkwiz.update = this_update;

  pthread_mutex_unlock(&clkwiz_mutex);
}

void clk_divide_all(void *clk_wiz, uint8_t divide)
{
  pthread_mutex_lock(&clkwiz_mutex);
  uint8_t i = 0;
  while(i < clkwiz.active)
  {
    if(clk_wiz == clkwiz.clkwiz_map[i])
      break;
    ++i;
  }

  *((uint32_t *)(clkwiz.clkwiz_map[i] + CLK_CONF_REG0)) = (*((uint32_t *)(clkwiz.clkwiz_map[i] + CLK_CONF_REG0)) & (0xFFFFFF00)) | divide;

  (*clkwiz.data)[i].reg_value[0] = divide;

  clkwiz.tasks[i].all |= CLKWIZ_DIVIDE_ALL;

  pthread_mutex_unlock(&clkwiz_mutex);
}

void clk_terminate()
{
  pthread_join(clkwiz.thread, NULL);
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
  //(*clkwiz.data)[i].reg_value = multiply;

  pthread_mutex_unlock(&clkwiz_mutex);
}


void read_all_clk_reg(void *clk_wiz)
{
  pthread_mutex_lock(&clkwiz_mutex);
  uint8_t i = 0;
  while(i < clkwiz.active)
  {
    if(clk_wiz == clkwiz.clkwiz_map[i])
      break;

    ++i;
  }

  for(uint8_t j = 0; j < 23; ++j)
  {
    printf("Clock Config Reg %d: %#010x\n", j, *((uint32_t *)(clkwiz.clkwiz_map[i] + j*0x4+CLK_CONF_REG0)));
  }

  pthread_mutex_unlock(&clkwiz_mutex);
}