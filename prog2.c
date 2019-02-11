// This the original test case posted on course website, but we do not use it because they may not be launched at meantime.
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    ticket(200); // write your own function here
    int i, k;
    const int loop = 43000;
    for (i = 0; i < loop; i++)
        for (k = 0; k < loop; k++)
            asm("nop");
    exit();
}
