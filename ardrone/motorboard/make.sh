#!/bin/bash 
arm-none-linux-gnueabi-g++ -lpthread -o ../bin/motorboard motorboard.c mot.c ../gpio/gpio.c ../util/util.c main_motorboard.c