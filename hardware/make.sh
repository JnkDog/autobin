# exit when any command fails
set -e

echo -e "${RED_COLOR}================ Starting server ================${RESET}"
cd ..
cd ${HARDWARE_PATH}
mkdir build && cd build
cmake ..
make
# ./hardware