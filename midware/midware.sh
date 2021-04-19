#! /bin/bash

# Date：21:00 2021-4-16
# Author: Create by Pengchong Wang 
# Description: This script function is to drive the midware
# Version： 1.0

# exit when any command fails
set -e

echo -e "${RED_COLOR}================ Starting midware ================${RESET}"
cd ..
cd ${MIDWARE_PATH}
mkdir build && cd build
cmake ..
make 
# ./midware