#!/bin/bash
set -e

cd "$(dirname "$0")"

echo "=== STEP 1: CLEANUP ==="
rm -rf build/ .qt/
find . -name "*.moc" -delete 2>/dev/null
find . -name "*.moc.d" -delete 2>/dev/null
find . -name "CMakeCache.txt" -delete 2>/dev/null
find . -type d -name "CMakeFiles" -exec rm -rf {} + 2>/dev/null
find . -type d -name "overseer_autogen" -exec rm -rf {} + 2>/dev/null
find . -name "*.o" -delete 2>/dev/null
echo "Cleanup complete"

echo ""
echo "=== STEP 2: CONFIGURE ==="
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..

echo ""
echo "=== STEP 3: BUILD ==="
make -j$(nproc)

echo ""
echo "=== STEP 4: INSTALL ==="
if [ -f liboverseer.so ]; then
    echo "Build successful: $(ls -lh liboverseer.so | awk '{print $5}')"
    echo "Installing to /usr/local/lib/obs-plugins/liboverseer-plugin.so"
    sudo cp liboverseer.so /usr/local/lib/obs-plugins/liboverseer-plugin.so
    sudo chmod 644 /usr/local/lib/obs-plugins/liboverseer-plugin.so
    echo "Installation complete"
else
    echo "ERROR: liboverseer.so not found!"
    exit 1
fi

echo ""
echo "=== REBUILD COMPLETE ==="
echo "Run: pbmobs"
