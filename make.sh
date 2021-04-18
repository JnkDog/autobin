#! /bin/bash

# Date：16:49 2021-4-14 
# Author: Create by Kuihong Chen
# Description: This script function is compling the AutoBin project
# Version： 1.0

# ATTENTION: MAKE SURE YOU ARE IN AUTOBIN DIRECTORY

# exit when any command fails
set -e

RED_COLOR='\E[1;31m'
RESET='\E[0m'

WEB_PATH=./autobin-web-demo/
SERVER_PATH=./server/
CAMERA_PATH=./camera/
MIDWARE_PATH=./midware/

echo "Starting script at $(date "+%Y-%m-%d  %H:%M:%S")"

echo "The script is deploying AutoBin, plz waiting..."

echo -e "${RED_COLOR}================ Starting web ================${RESET}"

cd ${WEB_PATH}
npm install

echo -e "${RED_COLOR}================ Starting server ================${RESET}"
cd ..
cd ${server}
npm install

echo -e "${RED_COLOR}================ Starting midware ================${RESET}"
cd ..
cd ${MIDWARE_PATH}
mkdir build && cd build
cmake ..
make 

echo -e "${RED_COLOR}================  Starting camera ================${RESET}"
cd ..
cd ${CAMERA_PATH}
mkdir build && cd build
cmake ..
make 

echo -e "${RED_COLOR}================ Make OK!!! ================ ${RESET}"
echo "Starting script at $(date "+%Y-%m-%d  %H:%M:%S")"