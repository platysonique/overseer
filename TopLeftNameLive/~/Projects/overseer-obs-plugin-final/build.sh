#!/bin/bash

# Overseer OBS Plugin Build Script
# This script builds the plugin with proper Qt moc processing

echo "Building Overseer OBS Plugin..."
echo "================================"

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
echo "Note: CMAKE_AUTOMOC=ON is critical for Qt Q_OBJECT processing"
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
echo "📋 Verification steps:"
echo "1. Check vtable symbols: nm -D ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so | grep OverseerDock"
echo "   Expected: Defined symbol (V _ZTV12OverseerDock)"
echo ""
echo "2. Launch OBS and check View → Docks → 'Overseer Control'"
echo ""
echo "3. Test overlay creation and SHOW/HIDE functionality"
echo ""
echo "📁 Plugin installed to: ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/"
echo ""
echo "⚠️  IMPORTANT: Requires OBS core modification for 'source_release' signal"
echo "   See README.md for details on OBS source code changes needed."

echo "Build completed at $(date)"