```
/ # insmod /work_BBB/kernelobj.ko                                               
[  827.262269] Kernel object: module loaded                                     
/ # echo -n "Hello" > /sys/class/kernelobj/list                                 
[  834.099752] Kernel object: rw_store: value = Hello                           
/ # echo -n "World" > /sys/class/kernelobj/list                                 
[  840.769417] Kernel object: rw_store: value = World                           
/ # echo -n "From" > /sys/class/kernelobj/list                                  
[  846.170526] Kernel object: rw_store: value = From                            
/ # echo -n "Honk" > /sys/class/kernelobj/list                                  
[  854.105666] Kernel object: rw_store: value = Honk                            
/ # cat /sys/class/kernelobj/list
[  863.384678] Kernel object: rw_show: Hello                                    
[  863.384678] World                                                            
[  863.384678] From                                                             
[  863.384678] Honk                                                             
Hello                                                                           
World                                                                           
From                                                                            
Honk                                                                            
/ # rmmod kernelobj                                                             
[  877.898482] Kernel object: Uninstalled the module                            
/ # 
```