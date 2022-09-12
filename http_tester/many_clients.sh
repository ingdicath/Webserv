#!/bin/bash
# test with: time ./many_clients.sh

for N in {1..100}
do
    ./test 3 &
done
wait
