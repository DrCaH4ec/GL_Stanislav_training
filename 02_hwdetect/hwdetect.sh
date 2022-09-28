#!/bin/bash

previous_devices=/tmp/previous_devices.txt
current_devices=/tmp/current_devices.txt

function clean_files() {
    rm -f $previous_devices $current_devices
    echo "Clean temporary files"
}

# TODO: not working
trap clean_files EXIT SIGHUP SIGINT SIGKILL SIGTERM SIGQUIT

lsusb > $previous_devices
i2cdetect -l >> $previous_devices
previous_dev_len=$(ls /dev | wc -l)
current_dev_len=$previous_dev_len

# TODO: Coding style
echo "Currently plugged devices:
* USBs:
$(lsusb)
* I2C:
$(i2cdetect -l)"

while inotifywait -qe create,delete /dev
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
done
