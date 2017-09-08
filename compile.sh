#!/bin/bash

g++ -c chess.c \
    -Os \
    -fPIC \
    -fno-jump-tables \
    -march=native

bash -c 'cd dump-elf-bytes; stack exec dump-elf-bytes -- ../chess.o > ../shellcode_bytes.py'
