# OVERSEER PLUGIN - BULLET-PROOF MASTER PLAN
## **EXACT USER REQUIREMENTS & IMPLEMENTATION SPECIFICATION**

---

## **🎯 CORE OBJECTIVE**
Create a single OBS Plugin called "Overseer Plugin" that provides dockable control board for beauty pageant overlay management with downstream keyer functionality.

---

## **📋 FUNCTIONAL REQUIREMENTS**

### **1. OBS INTEGRATION**
- Plugin loads automatically with OBS startup
- Appears as dockable control panel in OBS interface
- No external scripts, servers, or manual launching required
- Qt-based native OBS interface (not browser-based)

### **2. DOWNSTREAM KEYER FUNCTIONALITY**
- Overlays appear on ALL scenes regardless of current scene selection
- Function as global overlays that persist across scene changes
- Independent of scene-specific sources
- Always visible when enabled, regardless of scene switching

### **3. CONTROL BOARD ARCHITECTURE**
- Extensible dock panel that grows horizontally as overlays are added
- Single dropdown menu for overlay type selection
- "Add" button creates new overlay control module in the dock
- Horizontal arrangement of overlay control modules
- Move left/right buttons for rearranging overlay positions

### **4. OVERLAY POSITIONING SYSTEM**
Each overlay instance supports exactly 5 positioning options:
- **UpLeft**: Top-left corner positioning
- **UpRight**: Top-right corner positioning
- **BtLeft**: Bottom-left corner positioning
- **BtRight**: Bottom-right corner positioning
- **BtCenter**: Bottom-center positioning

### **5. MULTIPLE INSTANCE SUPPORT**
- Unlimited instances of each overlay type allowed
- Each instance is completely independent
- Separate control modules for each instance
- Independent properties and cue states
- No cross-contamination between instances

### **6. OVERLAY TYPES (STRICT SCOPE CONTROL)**
**ONLY these overlay types from existing workspace:**

#### **Logo (Sponsor Bug)**
- Parameters: Text content, text color, background color, font size
- Positioning: 5 position options (UpLeft, UpRight, BtLeft, BtRight, BtCenter)
- Independent show/hide states per instance

#### **Lower Third (On Air)**
- Parameters: Title text, subtitle text, title color, subtitle color
- Positioning: 5 position options (UpLeft, UpRight, BtLeft, BtRight, BtCenter)
- Independent show/hide states per instance

#### **Live Bug**
- Parameters: Top text, bottom text, top color, bottom color
- Positioning: 5 position options (UpLeft, UpRight, BtLeft, BtRight, BtCenter)
- Independent show/hide states per instance

#### **Big Lower Third**
- Parameters: Title text, subtitle text, title color, subtitle color
- Positioning: 5 position options (UpLeft, UpRight, BtLeft, BtRight, BtCenter)
- Independent show/hide states per instance

#### **Countdown (Circles)**
- Parameters: Time display (HH:MM:SS), text color, font size
- Positioning: 5 position options (UpLeft, UpRight, BtLeft, BtRight, BtCenter)
- Independent show/hide states per instance

### **7. DATA INGEST SYSTEM**
- CSV and TXT file import capability
- File picker integrated into control board
- Data format: Name, Age, State per contestant
- Universal data storage accessible by all overlay instances

### **8. AUTO-FILL FUNCTIONALITY**
- Type-ahead suggestions in text input fields
- Pulls from ingested contestant data
- Name + Age combination for title fields
- State for subtitle fields
- Editable text (not dropdown selections)
- On-the-fly modifications allowed

### **9. AUTO-FILL CONTROLS**
- Individual toggle switches for each overlay instance
- Enable/disable auto-fill per overlay module
- Independent auto-fill state per instance
- No global auto-fill settings

### **10. CUE SYSTEM**
- Show/Hide buttons per overlay instance
- Independent cue states - triggering one instance doesn't affect others
- Real-time state synchronization
- Visual feedback for current state

### **11. PERSISTENCE**
- All settings saved automatically
- Overlay configurations persist across OBS sessions
- Ingested data persists across OBS sessions
- Positioning preferences saved per instance
- Auto-fill toggle states saved per instance

---

## **🛠️ TECHNICAL IMPLEMENTATION SPECIFICATION**

### **PLUGIN ARCHITECTURE**
```
overseer-plugin/
├── src/
│   ├── overseer-plugin.cpp          # OBS plugin entry point
│   ├── control-dock.cpp             # Qt dock widget implementation
│   ├── overlay-instance.cpp         # Individual overlay management
│   ├── data-manager.cpp             # CSV/TXT parsing & storage
│   ├── position-manager.cpp         # 5-position coordinate system
│   ├── cue-system.cpp               # Independent show/hide states
│   └── persistence-manager.cpp      # OBS settings integration
├── data/
│   ├── control-ui.ui                # Qt UI designer file
│   ├── overlay-outputs/             # HTML output templates
│   │   ├── logo-output.html
│   │   ├── lower-third-output.html
│   │   ├── live-bug-output.html
│   │   ├── big-lower-3rd-output.html
│   │   └── countdown-output.html
│   └── default-config.json          # Plugin defaults
├── CMakeLists.txt                   # Build configuration
└── build.sh                         # Build script
```

### **QT CONTROL INTERFACE STRUCTURE**
```
Control Dock Layout:
┌─────────────────────────────────────────────────┐
│ OVERSEER PLUGIN CONTROL BOARD                   │
├─────────────────────────────────────────────────┤
│ [Overlay Type ▼] [Add Overlay]                  │
├─────────────────────────────────────────────────┤
│ ┌─── LOGO INSTANCE 1 ─────┐ ┌─── LOWER THIRD INSTANCE 2 ──┐ │
│ │ Position: [UpLeft ▼]     │ │ Position: [BtCenter ▼]       │ │
│ │ Text: [_____]           │ │ Title: [_____]               │ │
│ │ Color: [🎨]             │ │ Subtitle: [_____]            │ │
│ │ [Show] [Hide]           │ │ [Show] [Hide]                │ │
│ │ ◀ Move ▶              │ │ ◀ Move ▶                     │ │
│ │ [x] Auto-fill           │ │ [x] Auto-fill                │ │
│ └─────────────────────────┘ └──────────────────────────────┘ │
├─────────────────────────────────────────────────┤
│ DATA MANAGEMENT                                 │
│ [Choose File...] [Load Data] [Clear Data]       │
│ Contestants loaded: 25                         │
└─────────────────────────────────────────────────┘
```

### **POSITION COORDINATE SYSTEM**
```
Canvas Resolution: 1920x1080 (configurable)

UpLeft:     x=50,   y=50   (top-left with margin)
UpRight:    x=1470, y=50   (top-right with margin)
BtLeft:     x=50,   y=930  (bottom-left with margin)
BtRight:    x=1470, y=930  (bottom-right with margin)
BtCenter:   x=810,  y=930  (bottom-center)

Margin: 50px from edges
Coordinates adjust automatically for different canvas resolutions
```

### **DATA STRUCTURES**

#### **Contestant Data Structure:**
```cpp
struct Contestant {
    std::string name;
    int age;
    std::string state;
};
```

#### **Overlay Instance Structure:**
```cpp
struct OverlayInstance {
    int id;                           // Unique instance ID
    OverlayType type;                 // LOGO, LOWER_THIRD, etc.
    Position position;                 // UpLeft, UpRight, etc.
    std::map<std::string, std::string> parameters; // Text, colors, etc.
    bool visible;                     // Current show/hide state
    bool autoFillEnabled;             // Auto-fill toggle state
    std::string cueName;              // Optional cue identifier
};
```

#### **Position Enumeration:**
```cpp
enum Position {
    POSITION_UP_LEFT,
    POSITION_UP_RIGHT,
    POSITION_BOTTOM_LEFT,
    POSITION_BOTTOM_RIGHT,
    POSITION_BOTTOM_CENTER
};
```

### **AUTO-FILL LOGIC**
```
Input Field Type → Auto-fill Behavior:
- Title fields → "Name, Age" (e.g., "Amy Winehouse, 29")
- Subtitle fields → "State" (e.g., "Texas")
- Other fields → No auto-fill

Auto-fill triggers on:
- First character typed
- Minimum 1 character input
- Matching against loaded contestant data
- Alphabetical sorting of suggestions
```

### **CUE SYSTEM ARCHITECTURE**
```
Each overlay instance maintains independent state:
- Instance 1 Show/Hide affects only Instance 1
- Instance 2 Show/Hide affects only Instance 2
- No global show/hide commands
- No cue group functionality
- Manual control only per instance
```

### **PERSISTENCE IMPLEMENTATION**
```
OBS Settings Keys:
- "overseer/contestants" → JSON array of contestant data
- "overseer/instances" → JSON array of overlay instances
- "overseer/ui_state" → Dock panel layout preferences

Save Triggers:
- Parameter value changes
- Instance creation/deletion
- Position changes
- Auto-fill toggle changes
- Data import/clear operations
```

---

## **🔄 WORKFLOW SPECIFICATION**

### **Initial Plugin Load**
1. OBS loads overseer plugin
2. Plugin registers dock source
3. Qt dock widget created and added to OBS interface
4. Persistence manager loads saved data
5. Control board initializes with saved overlay instances
6. Data manager restores contestant data

### **Adding New Overlay Instance**
1. User selects overlay type from dropdown
2. User clicks "Add Overlay" button
3. New instance created with default parameters
4. Control module added to dock panel
5. Instance registered with cue system
6. Browser source created for overlay output
7. Instance saved to persistence

### **Positioning Overlay**
1. User selects position from dropdown (UpLeft, UpRight, etc.)
2. Position coordinates calculated for current canvas
3. Overlay output repositioned immediately
4. Position saved to instance persistence

### **Data Import Process**
1. User clicks "Choose File" button
2. File picker opens (CSV/TXT filter)
3. Selected file validated for format
4. Data parsed into contestant structures
5. Contestants stored in data manager
6. Auto-fill system updated with new data
7. Data saved to persistence
8. UI updated with contestant count

### **Auto-fill Operation**
1. User types in text input field
2. Auto-fill system searches contestant data
3. Matching suggestions generated
4. Best match inserted into field
5. Field remains editable for modifications
6. No dropdown selection required

### **Cue Operation**
1. User clicks Show/Hide button on specific instance
2. Instance visibility state toggled
3. Overlay output show/hide state updated
4. Visual feedback updated in control
5. State saved to persistence
6. Other instances unaffected

### **Move Operation**
1. User clicks move left/right button on instance
2. Instance position in control board updated
3. Adjacent instances shift accordingly
4. UI layout recalculated
5. Layout preferences saved to persistence

---

## **🎯 SUCCESS VALIDATION CRITERIA**

### **Functional Validation**
- [ ] Plugin loads automatically with OBS
- [ ] Dock panel appears in OBS interface
- [ ] Dropdown shows all 5 overlay types
- [ ] Add button creates new control modules
- [ ] Control board expands horizontally
- [ ] Move buttons rearrange module positions
- [ ] Each module has independent controls
- [ ] 5 position options available per module
- [ ] Multiple instances of same type allowed
- [ ] CSV/TXT import works correctly
- [ ] Auto-fill suggestions appear when typing
- [ ] Auto-fill toggle works per instance
- [ ] Show/Hide affects only specific instance
- [ ] Overlays appear on all scenes (downstream keyer)
- [ ] Positioning works in all 5 locations
- [ ] All settings persist across OBS sessions

### **Scope Validation**
- [ ] NO features beyond existing overlay parameters
- [ ] NO additional positioning options
- [ ] NO global controls or cue groups
- [ ] NO browser-based interfaces
- [ ] NO external script dependencies
- [ ] NO scope creep beyond specified requirements

### **Performance Validation**
- [ ] No OBS performance impact with multiple overlays
- [ ] Instant response to control changes
- [ ] Smooth scene transitions with overlays
- [ ] Memory usage remains reasonable
- [ ] No crashes or instability

---

## **🚫 STRICT CONSTRAINTS**

### **ABSOLUTE NO-GO ITEMS**
- ❌ Browser-based control interfaces
- ❌ External script launching requirements
- ❌ Server dependencies of any kind
- ❌ Global show/hide controls
- ❌ Cue groups or batch operations
- ❌ Additional positioning options beyond 5 specified
- ❌ New overlay types beyond existing 5
- ❌ Additional parameters beyond existing overlays
- ❌ Dropdown selections for text input (must be editable)
- ❌ Timeline-based or complex cue systems
- ❌ Any scope creep beyond exact specifications

### **QUALITY ASSURANCE**
- ✅ Every overlay instance completely independent
- ✅ No cross-contamination between instances
- ✅ Downstream keyer functionality verified
- ✅ All 5 positions tested and working
- ✅ Persistence tested across OBS sessions
- ✅ Auto-fill tested with real contestant data
- ✅ Performance tested with multiple overlays active

---

## **📋 IMPLEMENTATION CHECKLIST**

### **Phase 1: Plugin Foundation**
- [ ] Create C++ OBS plugin project structure
- [ ] Implement OBS plugin loading mechanism
- [ ] Create Qt dock widget base class
- [ ] Set up build system (CMake)

### **Phase 2: Control Board UI**
- [ ] Implement extensible dock panel
- [ ] Add overlay type dropdown (5 options)
- [ ] Create Add button functionality
- [ ] Implement horizontal layout system
- [ ] Add move left/right buttons

### **Phase 3: Overlay Instance Management**
- [ ] Create overlay instance data structure
- [ ] Implement instance creation/deletion
- [ ] Add position selection (5 options)
- [ ] Create parameter controls per overlay type
- [ ] Implement show/hide buttons per instance

### **Phase 4: Positioning System**
- [ ] Define 5 position coordinate system
- [ ] Implement position selection UI
- [ ] Add coordinate calculation logic
- [ ] Test positioning on different canvas sizes

### **Phase 5: Data Management**
- [ ] Implement CSV parser
- [ ] Implement TXT parser
- [ ] Create data storage system
- [ ] Add file picker to UI
- [ ] Implement data persistence

### **Phase 6: Auto-fill System**
- [ ] Create auto-fill engine
- [ ] Implement type-ahead suggestions
- [ ] Add toggle switches per instance
- [ ] Integrate with text input fields
- [ ] Test with real contestant data

### **Phase 7: Downstream Keyer**
- [ ] Implement global overlay system
- [ ] Test overlay persistence across scenes
- [ ] Verify independent operation
- [ ] Performance optimization

### **Phase 8: Cue System**
- [ ] Implement independent show/hide states
- [ ] Add visual feedback
- [ ] Test cue isolation
- [ ] Real-time state synchronization

### **Phase 9: Persistence**
- [ ] Implement OBS settings integration
- [ ] Add save/load functionality
- [ ] Test persistence across sessions
- [ ] Data integrity validation

### **Phase 10: Integration Testing**
- [ ] Full OBS integration testing
- [ ] Multiple instance testing
- [ ] Scene transition testing
- [ ] Performance testing
- [ ] Edge case validation

---

## **🎯 FINAL DELIVERABLE**

**A single OBS plugin that provides:**
- Dockable control board with horizontal expansion
- Independent overlay instances with 5 positioning options
- Downstream keyer functionality across all scenes
- CSV/TXT data ingest with auto-fill
- Complete persistence and no external dependencies

**NO SCOPE CREEP. NO EXTRA FEATURES. EXACTLY AS SPECIFIED.**
