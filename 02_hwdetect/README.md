# HW detect
This script monitors plugged and unplugged devices. When such an event occurs,
it notifys user by the message in the terminal.

---

## Install
Make sure that you have resolved all dependencies. In order to run the script,
you should install the following tools packages:
```
i2c-tools
inotify-tools
```
After the instalation add the executable permissions to the [hwdetect.sh](./hwdetect.sh)
so that it can run in the console. In the directory, where the script lives, open
the terminal and run the following command:
```
sudo chmod +x ./hwdetect.sh
```
Now you can run the script itself using:
```
./hwdetect.sh
```

---

## Usage
At first, script prints already plugged devices:
```
Currently plugged devices:
* USBs:
Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 002: ID 04f2:b669 Chicony Electronics Co., Ltd HP HD Camera
Bus 001 Device 003: ID 8087:0aaa Intel Corp. 
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
* I2C:
i2c-3	unknown   	i915 gmbus dpb                  	N/A
i2c-10	unknown   	AUX D/DDI D/PHY D               	N/A
i2c-1	unknown   	SMBus I801 adapter at efa0      	N/A
i2c-8	unknown   	AUX B/DDI B/PHY B               	N/A
i2c-6	unknown   	i915 gmbus dpd                  	N/A
i2c-4	unknown   	i915 gmbus dpc                  	N/A
i2c-2	unknown   	Synopsys DesignWare I2C adapter 	N/A
i2c-0	unknown   	Synopsys DesignWare I2C adapter 	N/A
i2c-9	unknown   	AUX C/DDI C/PHY C               	N/A
i2c-7	unknown   	AUX A/DDI A/PHY A               	N/A
i2c-5	unknown   	i915 gmbus misc                 	N/A

```
Then it only prints plugged or unplugged devices:
```
/dev/ CREATE sg0
Plugged devices:
< Bus 002 Device 002: ID 8564:1000 Transcend Information, Inc. JetFlash
/dev/ DELETE,ISDIR bsg
Unplugged devices:
> Bus 002 Device 002: ID 8564:1000 Transcend Information, Inc. JetFlash
```
