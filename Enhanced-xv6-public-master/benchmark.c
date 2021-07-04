
#include "types.h"
#include "user.h"

#define number_of_processes 10
int arra_pid[number_of_processes + 5];
int length = 0;
int main(int argc, char *argv[])
{
  int j;
  for (j = 0; j < number_of_processes; j++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed\n");
      continue;
    }
    if (pid == 0)
    {
      volatile int i;
      for (volatile int k = 0; k < number_of_processes; k++)
      {
        if (k <= j)
        {
          sleep(200); //io time
        }
        else
        {
          for (i = 0; i < 100000000; i++)
          {
            ; //cpu time
          }
        }
      }
      //   printf(1, "Process: %d Finished\n", j);
      exit();
    }
    else
    {
      ;
      arra_pid[length] = pid;
      length++;
      //setp(100 - (20 + j), pid); // will only matter for PBS, comment it out if not implemented yet (better priorty for more IO intensive jobs)
    }
  }
  // for (int i = 0; i < length; i++)
  //{
  //setp(100 - (20 + i), arra_pid[length]);
  //}
  for (j = 0; j < number_of_processes + 5; j++)
  {
    wait();
  }
  exit();
}
