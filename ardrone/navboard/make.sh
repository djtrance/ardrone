#!/bin/bash
arm-none-linux-gnueabi-g++ -o ../bin/navboard ../util/util.c ../gpio/gpio.c navboard.c main_navboard.c