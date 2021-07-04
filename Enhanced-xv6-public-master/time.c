#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf(1, "USAGE time <PROCESS>\n");
        exit();
    }
    int pid;
    int wtime, rtime;
    pid = fork(); // creating the child process <process>

    if (pid == 0)
    {
        //child
        exec(argv[1], &argv[1]);
        printf(1, "EXECUTION OF %s FAILED \n", argv[1]);
        exit();
    }
    waitx(&wtime, &rtime);

    printf(1, "THE WAITING_TIME (EXCLUDING I/O TIME) OF %s IS %d\n", argv[1], wtime);
    printf(1, "THE RUNNING_TIME OF %s IS %d\n", argv[1], rtime);
    exit();
}
