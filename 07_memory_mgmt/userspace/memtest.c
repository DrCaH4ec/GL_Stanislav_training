// Created by CYB3RSP1D3R

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <alloca.h>
#include <sys/resource.h>

#define MAX_ITER ((int)64)

const static char *break_table = 
    "|----------------------------------------------------------------------------------------|";


uint64_t average_uint64(uint64_t *array, size_t size)
{
    uint64_t sum = 0;

    for (size_t i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

uint64_t timetest_alloca(size_t size, struct timespec *free_begin)
{
    int8_t *memory;
    struct timespec begin, end;

    timespec_get(&begin, TIME_UTC);
    memory = alloca(size);
    timespec_get(&end, TIME_UTC);
    timespec_get(free_begin, TIME_UTC);
    return end.tv_nsec - begin.tv_nsec;
}

int main(int argc, char **argv)
{
    size_t epochs_max = 100;

    if (argc > 1) {
        epochs_max = atoi(argv[1]);
        if (epochs_max <= 0) {
            printf("Error: wrong epochs\n");
            return 1;
        }
    }

    uint64_t alloc_sample[epochs_max];
    uint64_t free_sample[epochs_max];
    uint64_t alloc_max, alloc_min, alloc_avr;
    uint64_t free_max, free_min, free_avr;
    size_t mem_size;
    struct timespec begin, end;
    int8_t *memory;
    struct rlimit stack_size;
    getrlimit(RLIMIT_STACK, &stack_size);

    for (int test_count = 0; test_count < 6; test_count++) {
        printf("\nTest with size 2 ^ x + %c uisng:\n", test_count < 3 ? '0' : '1');
        printf("%s\n", test_count % 3 == 0 ? "malloc" : test_count % 2 == 0 ? "calloc" : "alloca");
        printf("%s\n", break_table);
        printf("| Iter | allocation time (ns)%18c | free time (ns)%24c |\n",  ' ', ' ');
        printf("%s\n", break_table);
        printf("| %4c | %12s %12s %12s | %12s %12s %12s |\n",
            ' ', "min", "max", "avr", "min", "max", "avr");
        printf("%s\n", break_table);

        for (int count = 0; count < MAX_ITER; count++) {
            alloc_min = UINT64_MAX;
            alloc_max = 0;
            mem_size = 2 << count + (test_count < 3 ? 0 : 1);
            if (stack_size.rlim_cur < mem_size) {
                printf("%s\n", break_table);
                printf("|%31cCannot allocate the memory%31c|\n", ' ', ' ');
                break;
            }
            for (size_t epoch = 0; epoch < epochs_max; epoch++) {

                // allocate
                if (test_count % 3 == 0) {
                    timespec_get(&begin, TIME_UTC);
                    memory = malloc(mem_size);
                    timespec_get(&end, TIME_UTC);
                } else if (test_count % 2 == 0) {
                    timespec_get(&begin, TIME_UTC);
                    memory = calloc(mem_size, 0x1);
                    timespec_get(&end, TIME_UTC);
                } else {
                    alloc_sample[epoch] = timetest_alloca(mem_size, &begin);
                    timespec_get(&end, TIME_UTC);
                    alloc_min = (alloc_sample[epoch] < alloc_min ? alloc_sample[epoch] : alloc_min);
                    alloc_max = (alloc_sample[epoch] > alloc_max ? alloc_sample[epoch] : alloc_max);
                    free_sample[epoch] = end.tv_nsec - begin.tv_nsec;
                    free_min = (free_sample[epoch] < free_min ? free_sample[epoch] : free_min);
                    free_max = (free_sample[epoch] > free_max ? free_sample[epoch] : free_max);
                    continue;
                }
                if (memory == NULL) {
                    printf("%s\n", break_table);
                    printf("|%31cCannot allocate the memory%31c|\n", ' ', ' ');
                    break;
                }
                alloc_sample[epoch] = end.tv_nsec - begin.tv_nsec;
                alloc_min = (alloc_sample[epoch] < alloc_min ? alloc_sample[epoch] : alloc_min);
                alloc_max = (alloc_sample[epoch] > alloc_max ? alloc_sample[epoch] : alloc_max);

                // free
                timespec_get(&begin, TIME_UTC);
                free(memory);
                timespec_get(&end, TIME_UTC);
                free_sample[epoch] = end.tv_nsec - begin.tv_nsec;
                free_min = (free_sample[epoch] < free_min ? free_sample[epoch] : free_min);
                free_max = (free_sample[epoch] > free_max ? free_sample[epoch] : free_max);
            }
            if (alloc_max == 0)
                break;

            alloc_avr = average_uint64(alloc_sample, epochs_max);
            free_avr = average_uint64(free_sample, epochs_max);
            printf("| %4d | %12lu %12lu %12lu | %12lu %12lu %12lu |\n",
                count, 
                alloc_min, alloc_max, alloc_avr,
                free_min, free_max, free_avr
            );
        }
        printf("%s\n", break_table);
    }

    return 0;
}
