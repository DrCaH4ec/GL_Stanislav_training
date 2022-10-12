# Hello, kernel
This linux kernel module says hello to the user once
or mulitple time depending from the parameter.

---

## How to install
In the project folder type
```
make KDIR=/lib/modules/`uname -r`/build
```
Make script uses KBuild for module building,
so it will do everything automaticaly. You should only indicate the `KDIR`,
the directory where modules Makefile is stored.
If you build for the other architecture than you use for
building, you can specify the `ARCH` and `CROSS_COMPILE`
during the call of make. For instance:
```
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```
This is for ARM processors. If this method is not working, then declare those values in terminal using `export` command. 

And do not forget to add the path to the compiler to `$PATH`.

Also, you can use `make tidy` and `make clean` to delete all built files.

Then, you should add a module to the kernel using command:
```
sudo insmod hellokernel.ko name=<username> hello_times=<value>
```
where `name` is your name and `hello_times` is how many times module will say hello again.
If `hello_times` is 0, then module says hello just once.

Default values are `name="username"` and `hello_times = 0`

---

## How to see the result
Module send the messages to the logs. To see them type the command
`dmesg`. To see the last messages use `tail -n` command, where
`n` is a number of the lasts print collumns.
```
dmesg | tail -20
```
Example of the use, where the command types 20 last collumns of kernel messages.

---

## How to uninstall
To uninstall the module type
```
sudo rmod hellokernel.ko
```
The module will print goodbye message before being deleted.
