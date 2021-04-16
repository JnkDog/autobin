#include <iostream>
#include <unistd.h> 
#include <stdint.h>
#include <stdio.h>
#include "servo.h"

int main() {

    servo sv =  servo();
    sv.set_servo_open (15);

}