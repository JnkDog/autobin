#! /bin/bash

# Date：14:51 2021-4-17
# Author: Create by Kuihong Chen
# Description: This script function is boosting the AutoBin project
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

echo "The script is boosting AutoBin, plz waiting..."

echo -e "${RED_COLOR}================ Boosting web ================${RESET}"

cd ${WEB_PATH}
npm start 

echo -e "${RED_COLOR}================ Boosting server ================${RESET}"
cd ..
cd ${server}
npm start 

echo -e "${RED_COLOR}================ Boosting midware ================${RESET}"
cd ..
cd ${MIDWARE_PATH}
cd build
./midware

echo -e "${RED_COLOR}================  Boosting camera ================${RESET}"
cd ..
cd ${CAMERA_PATH}
cd build
./camera

echo -e "${RED_COLOR}================ Boost OK!!! ================ ${RESET}"
echo "Starting script at $(date "+%Y-%m-%d  %H:%M:%S")"
