#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    ticket(300); // write your own function here
    int i, k;
    // j = 0;
    const int loop = 43000;
    for (i = 0; i < loop; i++)
        for (k = 0; k < loop; k++)
            asm("nop");
    exit();
}