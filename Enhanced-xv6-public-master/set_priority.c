#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    int priority, pid;

    if (argc < 3)
    {
        printf(1, "USAGE : set_priority <new_priority> <pid>\n");
        exit();
    }
    priority = atoi(argv[1]);
    pid = atoi(argv[2]);

    if (priority < 0 || priority > 100)
    {
        printf(1, "INVALID NEW PRIORITY [0-100] \n");
    }

    setp(priority, pid);
    exit();
}