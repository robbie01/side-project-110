#!/bin/bash
g++ -c main.cpp primitives.cpp -std=c++17
g++ main.o primitives.o -lSDL2
./a.out
