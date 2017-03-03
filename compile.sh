#!/usr/bin/env bash

g++ main.cpp Servo.cpp UartRpi.cpp -o servo.bin -lwiringPi /usr/local/lib/libboost_program_options.a