# Portable Workspace Setup Guide

## Overview

This workspace contains the Overseer OBS Plugin project and related overlay files. This guide will help you set up the workspace on a new machine.

## Workspace Structure

```
TopLeftNameLive/
├── ~/
│   ├── obs-plugins/overseer/          # Alternative plugin location
│   └── Projects/
│       ├── overseer-obs-plugin-final/ # ⭐ ACTIVE PROJECT - Use this one
│       ├── overseer-obs-plugin/       # Older version (for reference)
│       └── overseer-new-build-files/  # Development files
├── Live Bug _ Beauty - overlays.uno Control App*.html  # Overlay HTML files
├── TopLeftNameLive.css                # CSS styling
├── PORTABLE_SETUP.md                  # This file
├── QUICKSTART.md                      # Quick reference
└── clean-builds.sh                    # Script to clean build artifacts
```

## Active Project

**The main active project is**: `~/Projects/overseer-obs-plugin-final/`

This is the production-ready version with complete build documentation and working configuration.

## System Requirements

- **Operating System**: Linux (Ubuntu/Debian recommended)
- **OBS Studio**: Installed and configured
- **Build Tools**: CMake, Make, GCC/G++
- **Qt6**: Development libraries
- **OBS Development Headers**: Required for plugin compilation

## Step 1: Install Dependencies

### Ubuntu/Debian

```bash
# Update package list
sudo apt update

# Install build essentials
sudo apt install build-essential cmake

# Install Qt6 development packages
sudo apt install qt6-base-dev qt6-base-dev-tools

# Install OBS development headers
sudo apt install libobs-dev obs-studio-dev

# Verify installations
cmake --version
qmake6 --version
pkg-config --modversion libobs
```

### Other Linux Distributions

- **Fedora**: `sudo dnf install cmake qt6-qtbase-devel obs-studio-devel`
- **Arch**: `sudo pacman -S cmake qt6-base obs-studio`
- **openSUSE**: `sudo zypper install cmake libqt6-qtbase-devel obs-studio-devel`

## Step 2: OBS Core Modifications (Required)

**IMPORTANT**: The plugin requires a modification to OBS core source code. This must be done before building the plugin.

### Locate OBS Source Code

If you installed OBS from packages, you may need to:
1. Download OBS source code from https://github.com/obsproject/obs-studio
2. Or locate your OBS installation's source files

### Required Modification

**File**: `libobs/obs-source.c`

**Location 1**: Add to `source_signals[]` array:
```c
static const char *source_signals[] = {
    "void destroy(ptr source)",
    "void remove(ptr source)",
    // ... existing signals ...
    "void source_release(ptr source)",  // ← ADD THIS LINE
    NULL,
};
```

**Location 2**: Add signal emission in `obs_source_release()` function:
```c
void obs_source_release(obs_source_t *source)
{
    // ... existing code ...
    if (obs_ref_release(&control->ref)) {
        obs_source_dosignal(source, "source_release", "source_release");  // ← ADD THIS LINE
        obs_source_destroy(source);
        obs_weak_source_release(control);
    }
}
```

**After modification**: Rebuild and reinstall OBS.

## Step 3: Build the Plugin

### Navigate to Active Project

```bash
cd ~/Projects/overseer-obs-plugin-final
```

### Option A: Use Build Script (Recommended)

```bash
# Make script executable
chmod +x build.sh

# Run build script
./build.sh
```

The script will:
1. Check for CMakeLists.txt
2. Create build directory
3. Configure with CMake
4. Build the plugin
5. Install to OBS plugin directory

### Option B: Manual Build

```bash
# Create build directory
mkdir -p build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# Install (requires sudo)
sudo make install
```

## Step 4: Verify Installation

### Check Plugin File

```bash
ls -lh ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so
```

### Verify Qt Symbols

```bash
nm -D ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so | grep OverseerDock
```

**Expected output**: Should show `V _ZTV12OverseerDock` (V = defined, not U = undefined)

### Launch OBS

```bash
obs
```

**Verify**:
1. OBS launches without crashes
2. Go to **View → Docks → "Overseer Control"**
3. Dock should appear with overlay controls
4. Test overlay creation and SHOW/HIDE buttons

## Step 5: Known Issues & Fixes

### Hardcoded Paths

**Issue**: The source code contains hardcoded paths that may not exist on your machine.

**Location**: `~/Projects/overseer-obs-plugin-final/src/overseer-plugin.cpp`
- Line 203: `/home/papaya/Projects/circle.png`
- Line 493: `/home/papaya/Projects/circle.png`

**Fix**: Edit the file and replace with:
- A relative path from the workspace
- Or use `QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)`
- Or provide your own default image path

**Example fix**:
```cpp
// Instead of:
QString defaultImage = "/home/papaya/Projects/circle.png";

// Use:
QString defaultImage = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/circle.png";
// Or relative path:
QString defaultImage = QDir::currentPath() + "/data/defaults/circle.png";
```

### Build Fails with Qt Errors

**Symptoms**: CMake can't find Qt6 or moc errors

**Solutions**:
1. Verify Qt6 is installed: `qmake6 --version`
2. Check CMakeLists.txt has `set(CMAKE_AUTOMOC ON)`
3. Ensure Qt6 development packages are installed

### Plugin Won't Load

**Symptoms**: OBS doesn't show the plugin, or shows errors

**Solutions**:
1. Check plugin file exists and is executable
2. Verify vtable symbols (see Step 4)
3. Check OBS logs: `~/.config/obs-studio/logs/`
4. Ensure OBS core modifications are applied

### Runtime Crashes

**Symptoms**: OBS crashes when plugin loads or when using dock

**Solutions**:
1. Verify OBS core has `source_release` signal (see Step 2)
2. Check OBS logs for specific error messages
3. Ensure Qt widgets are created after OBS finishes loading (should be automatic)

## Step 6: Clean Build Artifacts (Optional)

If you want to clean build artifacts before transferring or committing:

```bash
# From workspace root
./clean-builds.sh
```

Or manually:
```bash
find . -type d -name "build" -exec rm -rf {} +
find . -name "*.so" -delete
find . -name "CMakeCache.txt" -delete
find . -name "cmake_install.cmake" -delete
```

## Troubleshooting

### CMake Can't Find OBS

**Error**: `Could not find a package configuration file provided by "libobs"`

**Solution**:
```bash
# Find OBS installation
pkg-config --variable=prefix libobs

# Or set manually in CMakeLists.txt:
set(OBS_INCLUDE_DIRS "/usr/include/obs" CACHE PATH "OBS include directory")
set(OBS_LIB_DIRS "/usr/lib" CACHE PATH "OBS library directory")
```

### Qt6 Not Found

**Error**: `Could not find a package configuration file provided by "Qt6"`

**Solution**:
```bash
# Install Qt6 development packages
sudo apt install qt6-base-dev qt6-base-dev-tools

# Verify
qmake6 --version
```

### Permission Denied During Install

**Error**: `Permission denied` when running `sudo make install`

**Solution**: Ensure you have sudo privileges, or install manually:
```bash
# Manual install
mkdir -p ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit
cp build/bin/64bit/overseer-plugin.so ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/
```

## Additional Resources

- **Full Build Report**: See `~/Projects/overseer-obs-plugin-final/BUILD_REPORT.md`
- **Detailed README**: See `~/Projects/overseer-obs-plugin-final/README.md`
- **OBS Plugin Development**: https://obsproject.com/docs/plugins.html
- **Qt6 Documentation**: https://doc.qt.io/qt-6/

## Quick Reference

**Active Project**: `~/Projects/overseer-obs-plugin-final/`
**Build Script**: `./build.sh`
**Plugin Output**: `~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so`
**OBS Logs**: `~/.config/obs-studio/logs/`

## Support

If you encounter issues:
1. Check OBS logs: `~/.config/obs-studio/logs/`
2. Verify all dependencies are installed
3. Ensure OBS core modifications are applied
4. Review BUILD_REPORT.md for known issues and solutions

