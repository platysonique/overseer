# Overseer Plugin for OBS Studio

**Modular Overlay Control with Downstream Keyer Functionality**

The Overseer Plugin provides a dockable control board for managing beauty pageant overlays in OBS Studio. Each overlay instance is completely independent with its own positioning, parameters, and cue states.

---

## 🎯 Key Features

### **Downstream Keyer Functionality**
- Overlays appear on ALL scenes regardless of current scene selection
- Global overlay system independent of scene-specific sources
- Always visible when enabled, regardless of scene switching

### **Independent Overlay Instances**
- Unlimited instances of each overlay type
- Each instance has completely isolated controls
- No cross-contamination between instances
- Individual cue states (show/hide)

### **5-Position Coordinate System**
- **UpLeft**: Top-left corner
- **UpRight**: Top-right corner
- **BtLeft**: Bottom-left corner
- **BtRight**: Bottom-right corner
- **BtCenter**: Bottom-center positioning

### **Data Management**
- CSV and TXT file import for contestant data
- Auto-fill suggestions when typing
- Universal data accessible by all overlay instances
- Persistent storage across OBS sessions

### **Data File Formats**

#### **CSV Format**
```csv
Name,Age,State
Amy Winehouse,29,Texas
Sarah Johnson,25,California
```

#### **TXT Format**
```
Amy Winehouse
29
Texas

Sarah Johnson
25
California
```

### **Modular Control Board**
- Horizontal expansion as overlays are added
- Move left/right buttons for rearranging controls
- Independent auto-fill toggles per overlay
- Real-time parameter updates

---

## 📦 Installation

### **Prerequisites**
- OBS Studio (latest version)
- CMake (3.15+)
- Qt5 development libraries
- C++17 compatible compiler

### **Build and Install**
```bash
cd ~/Projects/overseer-obs-plugin
chmod +x build.sh
./build.sh
```

### **Verify Installation**
1. Start OBS Studio
2. Check View → Docks menu for "Overseer Plugin"
3. The dock should appear automatically

---

## 🎮 Usage Guide

### **Initial Setup**
1. **Launch OBS Studio**
2. **Find Overseer Plugin** in View → Docks
3. **Dock the control board** in your preferred location
4. **The interface is ready to use**

### **Adding Overlays**
1. **Select overlay type** from dropdown (Logo, Lower Third, etc.)
2. **Click "Add Overlay"** button
3. **New control module** appears in the dock
4. **Copy the file path** shown for OBS browser source
5. **Add Browser source** in OBS with the copied path

### **Positioning Overlays**
Each overlay supports exactly 5 positions:
- Select position from dropdown in each control module
- Coordinates automatically calculated for current canvas
- Overlays reposition instantly

### **Loading Contestant Data**
1. **Click "Choose File..."** in Data Management section
2. **Select CSV or TXT file** with contestant data
3. **Click "Load Data"** to import
4. **Auto-fill activates** when typing in text fields

### **Controlling Overlays**
- **SHOW/HIDE buttons**: Independent per overlay instance
- **Move ◀ ▶ buttons**: Rearrange control modules
- **Parameter controls**: Real-time updates
- **Auto-fill toggle**: Enable/disable per instance

### **Auto-fill Behavior**
- **Title fields**: "Name, Age" (e.g., "Amy Winehouse, 29")
- **Subtitle fields**: "State" (e.g., "Texas")
- **Editable**: All suggestions remain editable
- **Per-instance**: Auto-fill toggle per overlay

---

## 📊 Data File Formats

### **CSV Format**
```csv
Name,Age,State
Amy Winehouse,29,Texas
Sarah Johnson,25,California
```

### **TXT Format**
```
Amy Winehouse
29
Texas

Sarah Johnson
25
California
```

### **Auto-fill Behavior**
- **Title fields**: "Name, Age" (e.g., "Amy Winehouse, 29")
- **Subtitle fields**: "State" (e.g., "Texas")
- **Editable**: All suggestions remain editable
- **Per-instance**: Auto-fill toggle per overlay

---

## 🛠️ Overlay Specifications

### **Logo (Sponsor Bug)**
- **Parameters**: Text, text color, background color, font size
- **Use**: Corner sponsor branding
- **Positions**: All 5 available

### **Lower Third (On Air)**
- **Parameters**: Title, subtitle, title color, subtitle color
- **Use**: Name and location display
- **Positions**: All 5 available

### **Live Bug**
- **Parameters**: Top text, bottom text, colors
- **Use**: Live broadcast indicators
- **Positions**: All 5 available

### **Big Lower Third**
- **Parameters**: Title, subtitle, colors
- **Use**: Major announcements
- **Positions**: All 5 available

### **Countdown (Circles)**
- **Parameters**: Time (HH:MM:SS), color, font size
- **Use**: Event countdown timers
- **Positions**: All 5 available

---

## 🎯 Technical Architecture

### **Plugin Structure**
```
overseer-plugin/
├── src/overseer-plugin.cpp     # Main plugin implementation
├── CMakeLists.txt              # Build configuration
├── build.sh                    # Build script
├── data/
│   ├── locale/en-US.ini        # Localization
│   └── overlays/               # Output templates (future)
└── README.md                   # Documentation
```

### **Qt Interface**
- **QDockWidget**: Native OBS dock integration
- **QHBoxLayout**: Horizontal control expansion
- **QScrollArea**: Scrollable control area
- **Independent controls**: Each overlay instance isolated

### **Data Persistence**
- **QSettings**: OBS settings integration
- **JSON storage**: Contestant and overlay data
- **Automatic save**: All changes persisted immediately

### **Coordinate System**
```
Canvas: 1920x1080 (configurable)
Margin: 50px from edges

UpLeft:    (50, 50)
UpRight:   (1870, 50)
BtLeft:    (50, 1030)
BtRight:   (1870, 1030)
BtCenter:  (960, 1030)
```

---

## 🔧 Troubleshooting

### **Plugin Not Loading**
- Check OBS log for error messages
- Verify Qt5 libraries are installed
- Confirm plugin built successfully

### **Dock Not Appearing**
- Check View → Docks menu
- Restart OBS after installation
- Verify plugin installed in correct directory

### **Overlays Not Showing**
- Confirm Browser source URL is correct
- Check OBS browser source settings
- Verify overlay output files exist

### **Auto-fill Not Working**
- Confirm data loaded successfully
- Check auto-fill toggle is enabled
- Verify data format matches specifications

---

## 📝 Development Notes

### **Scope Limitations**
- ✅ Exactly 5 overlay types from existing workspace
- ✅ Only parameters present in current overlays
- ✅ Exactly 5 positioning options
- ✅ Independent instance operation
- ✅ No global controls or cue groups

### **Code Standards**
- C++17 standard
- Qt5 framework
- OBS plugin API compliance
- Modular architecture
- Comprehensive error handling

### **Future Extensions**
- Additional overlay templates
- Custom positioning coordinates
- Advanced cue timing
- Network synchronization

---

## 📄 License & Credits

**Author:** Pombomb Media
**Version:** 1.0.0
**License:** MIT (assumed)

**Built for:** Beauty pageant live streaming
**Tested with:** OBS Studio 31.x

---

## 📋 Additional Documentation

See `README_POMBOMB.md` for detailed information about:
- Web-based control interface concepts
- Extended usage examples
- Alternative architectural approaches

*For technical support or feature requests, please refer to the master plan specification.*


