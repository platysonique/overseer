#!/bin/bash
# Complete cleanup script for Overseer plugin build

cd "$(dirname "$0")"

echo "=== CLEANING BUILD ARTIFACTS ==="
rm -rf build/
rm -rf .qt/
find . -name "*.moc" -delete
find . -name "*.moc.d" -delete
find . -name "CMakeCache.txt" -delete
find . -name "cmake_install.cmake" -delete
find . -type d -name "CMakeFiles" -exec rm -rf {} + 2>/dev/null
find . -type d -name "overseer_autogen" -exec rm -rf {} + 2>/dev/null
find . -name "*.o" -delete
find . -name "*.so" -delete

echo "=== CLEANUP COMPLETE ==="
echo "Run: mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j\$(nproc)"
