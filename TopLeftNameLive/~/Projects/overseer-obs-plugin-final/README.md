# Overseer OBS Plugin - Build Report & Guide

## Executive Summary

The Overseer OBS Plugin provides modular overlay controls for live streaming. This document details the complete build process, issues encountered, and solutions implemented based on extensive debugging sessions.

## Project Overview

**Purpose**: Control streaming overlays with a dock interface supporting multiple overlay types:
- Logo overlays
- Lower third graphics
- Live bug displays
- Big lower thirds
- Countdown timers

**Architecture**:
- Qt-based OBS dock widget
- Browser source integration for overlay display
- Modular overlay management system
- Settings persistence

## Build Issues Encountered & Solutions

### Issue 1: Qt VTable Symbol Missing

**Error**: `undefined symbol: _ZTV12OverseerDock`

**Root Cause**: Qt's Meta-Object Compiler (moc) was not processing the `OverseerDock` class because `CMAKE_AUTOMOC` was not enabled.

**Evidence**:
```bash
nm -D liboverseer-plugin.so | grep OverseerDock
# Result: U _ZTV12OverseerDock (undefined)
```

**Solution**:
```cmake
# CMakeLists.txt - Add this line
set(CMAKE_AUTOMOC ON)
```

**Why This Happens**: Classes with `Q_OBJECT` macro require moc processing to generate vtables and meta-object code. Without `CMAKE_AUTOMOC`, the Qt build system doesn't automatically run moc.

### Issue 2: Soundboard Plugin Crash

**Error**: `signal_handler_connect: signal 'source_release' not found`

**Root Cause**: The soundboard plugin expected a "source_release" signal that didn't exist in the OBS core.

**Solution**: Added the missing signal to OBS source signal declarations:
```c
// In obs-source.c, source_signals[] array:
"void source_release(ptr source)",
```

**Implementation**:
```c
// Signal emission when source is released:
obs_source_dosignal(source, "source_release", "source_release");
```

### Issue 3: Scene Loading Configuration

**Error**: `No scene file found, creating default scene`

**Root Cause**: OBS configuration pointed to wrong scene collection path (Flatpak vs native paths).

**Solution**:
1. Correct `SceneCollections` path in `global.ini`
2. Create `scenes.json` index file

## Final Build Process

### Prerequisites

```bash
# Ubuntu/Debian dependencies
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libobs-dev

# OBS development headers
sudo apt install obs-studio-dev
```

### Source Structure

```
overseer-obs-plugin/
├── CMakeLists.txt          # Build configuration
├── src/
│   ├── overseer-dock.h     # Qt dock widget header
│   └── overseer-plugin.cpp # Plugin implementation
└── data/
    └── locale/             # Localization files
```

### CMakeLists.txt (Critical Configuration)

```cmake
cmake_minimum_required(VERSION 3.15)
project(overseer-plugin VERSION 1.0.0)

# ENABLE AUTOMOC - CRITICAL FOR Q_OBJECT PROCESSING
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(OBS REQUIRED)

# Source files
set(SOURCES src/overseer-plugin.cpp)

# Headers (for moc processing)
set(HEADERS src/overseer-dock.h)

add_library(${PLUGIN_NAME} MODULE ${SOURCES})

target_include_directories(${PLUGIN_NAME} PRIVATE
    ${OBS_INCLUDE_DIRS}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    OBS::obs-frontend-api
    OBS::libobs
    Qt6::Core
    Qt6::Widgets
)

# Output to correct plugin directory
set_target_properties(${PLUGIN_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/64bit"
)
```

### Plugin Implementation Highlights

#### Header (overseer-dock.h)
```cpp
class OverseerDock : public QDockWidget {
    Q_OBJECT  // Requires moc processing

public:
    OverseerDock(QWidget* parent = nullptr);
    ~OverseerDock();

private slots:
    void onAddOverlayClicked();
    void onIngestDataClicked();

private:
    QList<OverlayInstance> overlays_;
    QComboBox* overlayTypeSelect_;
    QPushButton* addOverlayBtn_;
    QScrollArea* scrollArea_;
    // ... UI components
};
```

#### Plugin Entry Points (overseer-plugin.cpp)
```cpp
// Required OBS module functions
MODULE_EXPORT uint32_t obs_module_ver(void) {
    return LIBOBS_API_VER;
}

MODULE_EXPORT void obs_module_set_pointer(obs_module_t *module) {
    // OBS internal use
}

// Event handling for safe Qt initialization
void on_obs_ready(enum obs_frontend_event event, void* data) {
    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
        // Create dock after OBS is fully loaded
        QTimer::singleShot(1000, []() {
            auto* main_window = static_cast<QMainWindow*>(
                obs_frontend_get_main_window());
            if (main_window) {
                dock = new OverseerDock(main_window);
                obs_frontend_add_dock(dock);
            }
        });
    }
}
```

## OBS Core Modifications Required

### Source Signal Addition

**File**: `libobs/obs-source.c`

**Location**: `source_signals[]` array

**Addition**:
```c
static const char *source_signals[] = {
    "void destroy(ptr source)",
    "void remove(ptr source)",
    // ... existing signals ...
    "void source_release(ptr source)",  // ← ADD THIS LINE
    NULL,
};
```

**File**: `libobs/obs-source.c`

**Location**: `obs_source_release()` function

**Addition**:
```c
void obs_source_release(obs_source_t *source)
{
    // ... existing code ...
    if (obs_ref_release(&control->ref)) {
        obs_source_dosignal(source, "source_release", "source_release");
        obs_source_destroy(source);
        obs_weak_source_release(control);
    }
}
```

## Build Commands

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# Install
sudo make install
```

## Installation & Testing

### Plugin Installation
```bash
# Copy to OBS plugin directory
cp build/bin/64bit/overseer-plugin.so ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/
```

### Verification
```bash
# Check for vtable symbols
nm -D ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so | grep OverseerDock

# Expected output:
# 000000000000bb30 V _ZTV12OverseerDock  ← Defined (V), not undefined (U)
```

### Runtime Testing
```bash
# Launch OBS
obs

# Verify:
# 1. "Overseer Control" appears in View → Docks menu
# 2. Dock contains overlay type dropdown and Add Overlay button
# 3. No crashes during startup or usage
```

## Troubleshooting

### Build Fails with Qt Errors
- Ensure `CMAKE_AUTOMOC ON` is set
- Check Qt6 development packages are installed
- Verify header files include proper Qt includes

### Plugin Won't Load
- Check `nm -D` output for undefined Qt symbols
- Ensure moc generated files exist
- Verify OBS plugin directory structure

### Runtime Crashes
- Check OBS logs for signal connection errors
- Verify OBS core has source_release signal
- Ensure proper Qt initialization timing

## Key Lessons Learned

1. **Qt Integration**: Always enable `CMAKE_AUTOMOC` for classes with `Q_OBJECT`
2. **OBS Signals**: Third-party plugins may expect signals not in standard OBS
3. **Build System**: Separate headers from implementation for proper moc processing
4. **Timing**: Use event callbacks and timers for safe Qt widget creation in OBS
5. **Configuration**: OBS scene paths can be misconfigured between Flatpak/native installs

## Final Working Configuration

The plugin successfully:
- ✅ Loads without Qt vtable errors
- ✅ Creates functional dock interface
- ✅ Manages overlay instances
- ✅ Integrates with OBS browser sources
- ✅ Handles settings persistence
- ✅ Avoids OBS startup crashes

This implementation provides a robust foundation for streaming overlay management in OBS Studio.

