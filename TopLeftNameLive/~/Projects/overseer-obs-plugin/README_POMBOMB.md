# Overseer Plugin for OBS Studio

A comprehensive modular overlay control system for OBS Studio with data ingestion and auto-fill functionality.

## Features

- **Modular Overlay System**: Add, remove, and rearrange overlay elements
- **Real-time Control**: Live updates to overlay appearance and content
- **Data Ingestion**: Import contestant data from CSV or TXT files
- **Auto-fill**: Intelligent text suggestions based on contestant data
- **Horizontal Expansion**: Add elements side-by-side in the control interface
- **Persistence**: Settings and data survive OBS restarts

## Supported Overlays

1. **Logo (Sponsor Bug)**: Customizable sponsor text with colors and fonts
2. **Lower Third (On Air)**: Name/age and state display
3. **Live Bug**: Top/bottom text with location information
4. **Big Lower Third**: Large call-to-action overlay
5. **Countdown (Circles)**: Timer display with customizable time

## Installation

### Method 1: Direct Installation (Recommended)

The plugin is already installed in your OBS configuration:

```
~/.config/obs-studio/plugins/overseer-plugin/
```

### Method 2: Manual Installation

1. Copy the plugin files to your OBS plugins directory
2. Restart OBS Studio
3. The plugin will appear as "Overseer Plugin" in your sources

## Setup

1. **Start OBS Studio**
2. **Add Source**: Click the "+" button under Sources
3. **Select**: "Overseer Plugin"
4. **Configure**:
   - Web Server Port: 8080 (default)
   - Contestant Data File: Select CSV/TXT file (optional)

## Usage

### Basic Workflow

1. **Open Control Interface**:
   - Right-click the Overseer Plugin source
   - Select "Dock Source" → "Dock to new Source Dock Panel"
   - Or open: `http://localhost:8080/control.html`

2. **Add Overlays**:
   - Select overlay type from dropdown
   - Click "Add Overlay"
   - Control panel appears in the interface

3. **Customize Elements**:
   - Text content, colors, fonts, sizes
   - Position and timing controls
   - Real-time preview updates

4. **Load Contestant Data**:
   - Click "Choose File" in Data Management section
   - Select CSV or TXT file
   - Click "Load Data"

### Auto-fill System

When typing in text fields:

- **Title fields** (name): Auto-fills "Name, Age"
- **Subtitle fields** (state): Auto-fills "State"

**Example:**
```
Type: "A" → Auto-fills: "Amy Winehouse, 29"
Type: "S" → Auto-fills: "Texas" (for state fields)
```

### Data Formats

#### CSV Format:
```csv
Name,Age,State
Amy Winehouse,29,Texas
Sarah Johnson,25,California
```

#### TXT Format:
```
Amy Winehouse
29
Texas

Sarah Johnson
25
California
```

## Control Interface Layout

```
┌─────────────────────────────────────────────────┐
│ OVERSEER PLUGIN v1.0                           │
├─────────────────────────────────────────────────┤
│ Add Overlay: [Logo ▼] [Add]                     │
├─────────────────────────────────────────────────┤
│ ┌─── Logo Overlay ─────┬─── Lower Third ───┐    │
│ │ [Settings] [Move◄►] │ │ [Settings] [X]   │    │
│ │ Sponsor: SPONSOR    │ │ Name: __________ │    │
│ │ Color: █████████    │ │ Age: __________  │    │
│ │ [Show] [Hide]       │ │ State: _________ │    │
│ └─────────────────────┴────────────────────┘    │
│ [+ Add Element]                                  │
├─────────────────────────────────────────────────┤
│ Data Management                                  │
│ [Import CSV/TXT] [Clear Data]                    │
│ Contestants loaded: 25                          │
└─────────────────────────────────────────────────┘
```

## Overlay Parameters

### Logo Overlay
- **Text**: Custom sponsor text
- **Colors**: Text and background colors
- **Font**: Size and family
- **Position**: Top-left corner positioning

### Lower Third
- **Title**: Name and age display
- **Subtitle**: State/location display
- **Colors**: Separate colors for title/subtitle
- **Fonts**: Different sizes for title/subtitle

### Live Bug
- **Top Text**: "LIVE from" prefix
- **Bottom Text**: Location/state
- **Colors**: Separate colors for top/bottom
- **Font**: Consistent sizing

### Big Lower Third
- **Title**: Large call-to-action text
- **Subtitle**: Supporting text
- **Background**: Dual gradient colors
- **Fonts**: Large display sizes

### Countdown
- **Time**: HH:MM:SS format
- **Color**: Text color
- **Font**: Massive display size
- **Background**: Transparent or colored

## Technical Architecture

### Web Server
- **Port**: 8080 (configurable)
- **Protocol**: HTTP for static files
- **WebSocket**: Port 8081 for real-time communication

### File Structure
```
overseer-plugin/
├── bin/64bit/overseer.so    # Plugin executable
├── data/
│   ├── web/                 # Web interface
│   ├── overlays/            # Overlay templates
│   ├── contestants.json     # Persistent data
│   └── locale/              # Translations
```

### Communication
- **WebSocket**: Real-time control updates
- **HTTP**: Static file serving
- **JSON**: Configuration and data exchange

## Troubleshooting

### Plugin Not Loading
1. Check OBS log for errors
2. Verify file permissions on `overseer.so`
3. Ensure Python 3 is installed (WebSocket server requirement)

### Control Interface Not Accessible
1. Check if port 8080 is available
2. Verify web server is running
3. Check browser console for JavaScript errors

### Auto-fill Not Working
1. Confirm contestant data is loaded
2. Check WebSocket connection (port 8081)
3. Verify auto-fill is enabled per element

### Overlay Not Updating
1. Check WebSocket connection
2. Verify overlay configuration is being sent
3. Check browser developer tools for errors

## Development

### Building from Source
```bash
cd ~/Projects/Pombomb-OBS-Overseer
chmod +x build.sh
./build.sh
```

### Requirements
- **OBS Studio**: Latest version
- **Qt5**: For GUI components
- **Python 3**: For WebSocket server
- **websockets**: Python library (`pip install websockets`)

### Architecture
- **C++ Core**: OBS plugin interface
- **Python Backend**: WebSocket server and data management
- **HTML/CSS/JS**: Web-based control interface
- **JSON**: Data persistence and communication

## License

Custom development for Pombomb Media OBS integration.

## Support

For issues and feature requests, contact the development team.
