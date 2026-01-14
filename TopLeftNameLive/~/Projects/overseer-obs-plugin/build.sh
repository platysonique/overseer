#!/bin/bash

# Overseer Plugin Build Script
echo "Building Overseer Plugin for OBS..."
echo "===================================="

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the plugin root directory."
    exit 1
fi

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring build system..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build the plugin
echo "Building plugin..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Install to OBS plugin directory
echo "Installing plugin..."
sudo make install

if [ $? -ne 0 ]; then
    echo "Installation failed!"
    exit 1
fi

echo ""
echo "🎉 Overseer Plugin built and installed successfully!"
echo ""
echo "📋 Next steps:"
echo "1. Restart OBS Studio"
echo "2. Look for 'Overseer Plugin' in the View → Docks menu"
echo "3. Dock the control board in your preferred location"
echo "4. Start adding overlays and loading contestant data"
echo ""
echo "📁 Plugin installed to: $(pkg-config --variable=plugindir obs)/overseer-plugin/"
echo "📁 Data files in: $(pkg-config --variable=plugindir obs)/overseer-plugin/data/"
echo ""

echo "Build completed at $(date)"



