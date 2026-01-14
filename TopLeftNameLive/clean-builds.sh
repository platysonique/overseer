#!/bin/bash

# Clean Build Artifacts Script
# Removes all build artifacts from the workspace to prepare for transfer or clean rebuild

echo "Cleaning build artifacts..."
echo "============================"

# Get script directory (workspace root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Counter for removed items
REMOVED_COUNT=0

# Function to remove and count
remove_item() {
    if [ -e "$1" ] || [ -d "$1" ]; then
        echo "Removing: $1"
        rm -rf "$1"
        REMOVED_COUNT=$((REMOVED_COUNT + 1))
    fi
}

# Remove build directories
echo ""
echo "Removing build directories..."
find . -type d -name "build" -print0 | while IFS= read -r -d '' dir; do
    remove_item "$dir"
done

# Remove CMake cache files
echo ""
echo "Removing CMake cache files..."
find . -name "CMakeCache.txt" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove cmake install files
echo ""
echo "Removing cmake install files..."
find . -name "cmake_install.cmake" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove CMakeFiles directories
echo ""
echo "Removing CMakeFiles directories..."
find . -type d -name "CMakeFiles" -print0 | while IFS= read -r -d '' dir; do
    remove_item "$dir"
done

# Remove compiled libraries (.so files in build directories)
echo ""
echo "Removing compiled libraries..."
find . -type f -name "*.so" -path "*/build/*" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove object files
echo ""
echo "Removing object files..."
find . -type f -name "*.o" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove dependency files
echo ""
echo "Removing dependency files..."
find . -type f -name "*.d" -path "*/build/*" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove Makefiles in build directories
echo ""
echo "Removing Makefiles in build directories..."
find . -type f -name "Makefile" -path "*/build/*" -print0 | while IFS= read -r -d '' file; do
    remove_item "$file"
done

# Remove autogen directories
echo ""
echo "Removing autogen directories..."
find . -type d -name "*_autogen" -print0 | while IFS= read -r -d '' dir; do
    remove_item "$dir"
done

echo ""
echo "============================"
echo "Cleanup complete!"
echo "Removed $REMOVED_COUNT items"
echo ""
echo "The workspace is now clean and ready for transfer or rebuild."
echo "To rebuild, run: cd ~/Projects/overseer-obs-plugin-final && ./build.sh"

