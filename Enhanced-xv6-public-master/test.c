#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[])
{
    int no_of_process = 10;
    printf(1, "dddd");
    for (int i = 0; i < no_of_process; i++)
    {
        int pid = fork();

        if (pid == 0)
        {
            //child

            for (long long int j = 0; j < 1000000000; j++)
            {
                int x;
                x = (int)(153 * (double)(67 / 23) * 72.344);
                x = x % 10;
                x = x * 3;
                x = x / 6;
            }

            exit();
        }
    }

    for (int i = 0; i < 10 * no_of_process; i++)
    {
        wait();
    }

    exit();
}