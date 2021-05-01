#!/bin/sh
hashcat hash_set -a3 -o pass.txt --increment -m 1400 -D 2 --force -w 3 -O -1 '?u?l' -2 '?u?l?d' rules
