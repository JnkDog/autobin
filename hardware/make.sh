#! /bin/bash

# exit when any command fails
set -e

RED_COLOR='\E[1;31m'
RESET='\E[0m'

echo -e "${RED_COLOR}================ Hardware compiling ================${RESET}"
mkdir build && cd build
cmake ..
make
# ./demo