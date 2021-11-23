#!/bin/sh
sh-elf-g++ -S src/* -o asm.s -O1
cat asm.s
rm asm.s