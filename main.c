#include "mcp3008.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    initMcp3008();

    while(1)
    {
        fprintf(stderr, "AD-Wert Kanal %d ist %d \n", 0, getAdValue(0));
    }
    exitMcp3008();

    return 0;
}
