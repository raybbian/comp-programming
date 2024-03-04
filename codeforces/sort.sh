#!/bin/bash

# Loop through all files matching the extended pattern
for file in *[0-9][A-Z]*; do
    # Extract the number part before the first letter
    dir=$(echo "$file" | sed -E 's/([0-9]+)[A-Z].*/\1/')

    # Check if the directory exists, if not, create it
    if [ ! -d "$dir" ]; then
        mkdir "$dir"
    fi

    # Move the file to the corresponding directory
    mv "$file" "$dir/"
done

