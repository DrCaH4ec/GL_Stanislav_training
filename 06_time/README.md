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
/ # /work_BBB/user_code/time_user                                               
Kernel module 'time_kernel' is not loaded: Success                              
/ # insmod /work_BBB/kernel_module/time_kernel.ko                               
[    9.858215] time_kernel: loading out-of-tree module taints kernel.           
[    9.865324] time_kernel: module loaded                                       
/ # /work_BBB/user_code/time_user                                               
11.76040500                                                                     
/ # /work_BBB/user_code/time_user                                               
15.56039917                                                                     
/ # cat /sys/class/time_kernel/jiffies                                          
/ # /work_BBB/user_code/time_user                                               
38.76039667                                                                     
/ # cat /sys/class/time_kernel/jiffies                                          
4257/ #
/ # cat /sys/class/time_kernel/tick                                             
23300991961/ # 
```
Note: this is jiffies value starting from the system boot.
