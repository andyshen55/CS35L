#!/bin/sh

for i in $(ls /usr/bin | awk '(NR-705172144)%251 == 0')
do
   ldd '/usr/bin/'$i
done
