#!/usr/bin/env bash

usermod -a -G uucp $(whoami)

device="$(lsusb | grep -m1 "ST-LINK" | awk '{print $6}')"

idVendor="$( echo "${device}" | awk -F ":" '{print $1}')"
idProduct="$(echo "${device}" | awk -F ":" '{print $2}')"

file_content="# NUCLEO ST-LINK/V2.1
ATTRS{idVendor}==\"${idVendor}\", ATTRS{idProduct}==\"${idProduct}\",TAG+=\"uaccess\"
"

echo "${file_content}" | tee "/etc/udev/rules.d/70-st-link.rules" >/dev/null

udevadm control --reload-rules
