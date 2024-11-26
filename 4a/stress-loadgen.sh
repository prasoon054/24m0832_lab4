#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <server_ip> <server_port>"
    exit 1
fi

SERVER_IP=$1
SERVER_PORT=$2
# echo "Reached here"
for ((j = 1; j <= 64; j*=2)); do
    NUM_CLIENTS=$j
    gcc stress-client.c -o stress-client

    if [ $? -ne 0 ]; then
        echo "Compilation of server-client.c failed"
        exit 1
    fi
    
    echo "Starting $NUM_CLIENTS clients to connect to $SERVER_IP:$SERVER_PORT"

    start_time=$(date +%s%N)
    for ((i = 1; i <= $NUM_CLIENTS; i++)); do
        echo "Starting client #$i"
        ./stress-client $SERVER_IP $SERVER_PORT $i &
    done
    wait
    end_time=$(date +%s%N)
    duration=$(( (end_time-start_time) / 1000000 ))
    echo "All clients have finished."
    echo "Total time taken for $NUM_CLIENTS clients: $duration milliseconds"
    echo "$NUM_CLIENTS $duration" >> loadgen.txt
done
echo "Terminated"
