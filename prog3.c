#include "types.h"
#include "stat.h"
#include "user.h"

// int main(int argc, char *argv[])
// {
//     ticket(10); // write your own function here
//     int i, k;
//     // int j = 0;
//     const int loop = 43000;
//     for (i = 0; i < loop; i++)
//         for (k = 0; k < loop; k++)
//             asm("nop");
//     exit();
// }


int main(int argc, char *argv[])
{

    int Scheduler(void);

    Scheduler();
    return 0;
}

int Scheduler(void)
{

    int pid;
    int i, j, k;

    ticket(1000);
    for (i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid > 0)
        {
            continue;
        }
        else if (pid == 0)
        {

            ticket(1000 - 900 * (i%2));
            for (j = 0; j < 50000; j++)
            {
                for (k = 0; k < 50000; k++)
                {
                    asm("nop");
                }
            }
            printf(1, " \n  child# %d with %d tickets has finished!  \n ", getpid(), 1000 - 500 * (i%2));
            exit();
        }
        else
        {
            printf(2, "  \n  Error  \n ");
        }
    }

    if (pid > 0)
    {
        for (i = 0; i < 3; i++)
        {
            wait();
        }
    }
    exit();
    return 0;
}