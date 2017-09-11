#!/bin/bash

g++ -c chess.c \
    -O3 \
    -fPIC \
    -fno-jump-tables

bash -c 'cd dump-elf-bytes; stack exec dump-elf-bytes -- ../chess.o > ../shellcode_bytes.py'
