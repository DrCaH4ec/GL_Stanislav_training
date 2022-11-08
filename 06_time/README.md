# Time

The program return high resolution time from the boot in seconds.

## How to compile

Main folder contains Makefile that call the other makefiles from [kernelspace](./kernelspace) and [userspace](./userspace) folders.
In order to build all targets just print in the console:
```
make
```
Also there is a possibility to build separate parts using `user` or `kernel` parameters.

## How to clean

Call
```
make clean_all
```
to clean all files of the compilation result. If you need to clean only one of the parts, use `clean_user` or `clean_kernel`.

## Demo

```
Please press Enter to activate this console.                                    
/ # insmod /work_BBB/kernel_module/time_kernel.ko
[   43.665311] time_kernel: loading out-of-tree module taints kernel.
[   43.672575] time_kernel: module loaded
/ # cat /sys/class/time_kernel/absolute
45.791959047
/ # cat /sys/class/time_kernel/relative
3.46511067880
/ # cat /sys/class/time_kernel/absolute
52.307905131
/ # cat /sys/class/time_kernel/relative
0.53268192047
/ # /work_BBB/user_code/time_user
01/01/70 00:01:14.925103800
/ # /work_BBB/user_code/time_user
01/01/70 00:01:26.819635760
/ # cat /sys/class/time_kernel/absolute
89.164623635
```
Note: this is jiffies value starting from the system boot.
