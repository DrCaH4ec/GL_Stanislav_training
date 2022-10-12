#!/bin/bash

previous_devices=/tmp/previous_devices.txt
current_devices=/tmp/current_devices.txt

lsusb > $previous_devices
i2cdetect -l >> $previous_devices
previous_dev_len=$(ls /dev | wc -l)
current_dev_len=$previous_dev_len

echo "Currently plugged devices:
* USBs:
$(lsusb)
* I2C:
$(i2cdetect -l)"

while inotifywait -qqe create,delete /dev
do
    current_dev_len=$(ls /dev | wc -l)
    lsusb > $current_devices
    i2cdetect -l >> $current_devices
    if [ $current_dev_len -lt $previous_dev_len ]
    then
        echo "Unplugged devices:"
    else
        echo "Plugged devices:"
    fi
    echo "$(diff $current_devices $previous_devices | egrep '<|>')"
    lsusb > $previous_devices
    i2cdetect -l >> $previous_devices
    previous_dev_len=$current_dev_len
    sleep 2
done
