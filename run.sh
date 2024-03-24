#!/bin/bash

# Define the names of folders containing .vti files
folders=(
  "labeled_at_zfp"
  "labeled_at_sz3"
  "labeled_at2"
  "fixed_labeled_at_zfp"
)

# Loop through each specified folder
for folder in "${folders[@]}"; do
  # Find the .vti file within the folder
  file=$(find "./res/${folder}" -name '*.vti')

  # Check if the file exists
  if [[ -f "$file" ]]; then
    # Execute the 'app' program with the found file as input
    ./build/app -input "${file}"
  else
    # Output a message if the file is not found in the folder
    echo "File not found in ${folder}"
  fi
done

