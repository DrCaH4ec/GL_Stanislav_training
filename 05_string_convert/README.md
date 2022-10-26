# Install

```
insmod string_convert.ko buffer_len = 256
```

where `buffer_len` is a length of the buffers for sys fs and proc fs.
Default value is `256`.

# How to use

### PROC FS

```
/ # echo "hello, pepe =)" > /proc/string_convert                                
/ # cat /proc/string_convert                                                    
HELLO, PEPE =)                                                                  
/ # cat /proc/statistics                                                        
Total number of calls: 1                                                        
Total number of characters: 9                                                   
Total number of non-characters: 6                                               
Total number of conversions: 9                                                  
/ # 
```

### SYS FS

```
/ # echo "HELLO, PEPE =)" > /sys/class/string_convert/string_convert            
/ # cat /sys/class/string_convert/string_convert                                
hello, pepe =)                                                                  
/ # cat /sys/class/statistics/statistics                                        
Total number of calls: 2                                                        
Total number of characters: 18                                                  
Total number of non-characters: 12                                              
Total number of conversions: 18                                                 
/ # 
```

Note: statistics is one for both sys fs and proc fs.

# Checkpatch

```
make check_patch
/lib/modules/`uname -r`/build/scripts/checkpatch.pl -f --no-tree stringcase.c
total: 0 errors, 0 warnings, 256 lines checked

stringcase.c has no obvious style problems and is ready for submission.
```
