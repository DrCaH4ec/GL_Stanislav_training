// Made by CYB3RSP1D3R

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAX_INT_LEN ((int)20)

int main(int argc, char **argv)
{
    struct timespec ts;
    char str_time[18]; // time string takes exactly 18 characters including '\0'

    timespec_get(&ts, TIME_UTC);
    strftime(str_time, sizeof(str_time), "%D %T", localtime(&ts.tv_sec));

    printf("%s.%lu\n", str_time, ts.tv_nsec);

    return 0;
}
