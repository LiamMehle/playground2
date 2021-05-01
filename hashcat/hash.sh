#!/bin/sh

while IFS= read -r line; do
	echo -n $line | sha256sum | sed 's/ .*$//' >> hash_set;
done < passwd.txt 

