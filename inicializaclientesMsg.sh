#!/bin/sh
i=1
while [ "$i" -le 30 ]
do
    ./clienteMsg &
    i=$(($i+1))
done
exit 0