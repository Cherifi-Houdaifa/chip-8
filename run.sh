#!/bin/bash

gcc ./src/main.c ./src/sdl.c -o main.out -lSDL2 -g -no-pie &&
./main.out $1
