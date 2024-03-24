#!/bin/bash

# Check if the build directory exists
if [ -d build ]; then
  # If it exists, remove the old build directory
  echo "Removing existing build directory..."
  rm -rf build
fi

# Create a new build directory
echo "Creating new build directory..."
mkdir build

# Enter the build directory
cd build

# Configure the project with CMake. Adjust parameters as needed.
echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build the project using all available cores
echo "Building with make..."
make -j$(nproc)

echo "Build process completed."

