#!/bin/bash

gcc ./src/main.c -o main.out -lSDL2 -g -no-pie &&
./main.out
