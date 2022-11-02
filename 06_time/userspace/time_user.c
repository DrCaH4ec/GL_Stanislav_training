// Made by CYB3RSP1D3R

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_INT_LEN ((int)20)


bool isindir(DIR *dirp, char *filename);


int main(int argc, char **argv)
{
    DIR *dir_class = opendir("/sys/class");
    FILE *jiffies_file;
    FILE *tick_file;
    char jiffies_buffer[MAX_INT_LEN];
    char tick_buffer[MAX_INT_LEN];
    uint64_t jiffies_userspace, read_tick, sec, nsec;

    if (dir_class == NULL) {
        perror("Error: not enought permissions\nTry to run from sudo or directly from root");
        return EPERM;
    }
    if (!isindir(dir_class, "time_kernel")) {
        perror("Kernel module 'time_kernel' is not loaded");
        closedir(dir_class);
        return ENOENT;
    }
    closedir(dir_class);

    jiffies_file = fopen("/sys/class/time_kernel/jiffies", "r");
    tick_file = fopen("/sys/class/time_kernel/tick", "r");

    fgets(jiffies_buffer, MAX_INT_LEN, jiffies_file);
    fgets(tick_buffer, MAX_INT_LEN, tick_file);
    jiffies_userspace = atoi(jiffies_buffer);
    read_tick = atoi(tick_buffer);
    sec = jiffies_userspace / sysconf(_SC_CLK_TCK);
    nsec = (jiffies_userspace - sec * sysconf(_SC_CLK_TCK)) * 1000000 + read_tick;
    sec += read_tick / 1000000000;

    printf("%llu.%8llu\n", sec, nsec);

    fclose(jiffies_file);
    fclose(tick_file);
    return 0;
}

bool isindir(DIR *dirp, char *filename)
{
    struct dirent *entry;

    rewinddir(dirp); // in case if directory has been read before
    while ((entry = readdir(dirp)) != NULL) {
        if (!strcmp(entry->d_name, filename))
            return true;
    }
    rewinddir(dirp);
    return false;
}
