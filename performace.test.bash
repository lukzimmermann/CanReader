#!/bin/bash

# URL to call
url="http://10.0.0.203/raw-can"

# Get the start time in milliseconds (seconds + milliseconds)
start_time=$(($(date +%s) * 1000 + $(date +%N | cut -b1-3)))

# Number of requests
number=100

# Loop to make 100 requests
for i in $(seq 1 $number)
do
    # Make the GET request, suppress output with -s
    curl -s "$url"
done

# Get the end time in milliseconds (seconds + milliseconds)
end_time=$(($(date +%s) * 1000 + $(date +%N | cut -b1-3)))

# Calculate the total time taken
total_time=$((end_time - start_time))

# Calculate the time per request
time_per_request=$((total_time / number))

# Print out the total time consumed and the average time per request
echo "Total time for $number requests: $total_time ms"
echo "Average time per request: $time_per_request ms"