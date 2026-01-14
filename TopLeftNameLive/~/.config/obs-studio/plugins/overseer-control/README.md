# Overseer Control Interface for OBS

## How to Use in OBS

1. **Add Browser Source:**
   - In OBS, click the "+" button under Sources
   - Select "Browser"
   - Name it "Overseer Control"

2. **Configure Browser Source:**
   - **URL:** `http://localhost:8080/control.html`
   - **Width:** `400` (or desired width)
   - **Height:** `600` (or desired height)
   - **Shutdown source when not visible:** ✅ Checked

3. **Dock the Control Interface:**
   - Right-click the Browser source
   - Select "Dock Source" → "Dock to new Source Dock Panel"

## Features Available

### Overlay Management
- **Add Overlays:** Select type from dropdown and click "Add Overlay"
- **Control Overlays:** Show/Hide/Move/Remove individual overlays
- **Real-time Updates:** Changes apply immediately to overlay outputs

### Available Overlay Types
- **Logo (Sponsor Bug):** Text, colors, sizing
- **Lower Third (On Air):** Title, subtitle, colors
- **Live Bug:** Top/bottom text, positioning
- **Big Lower Third:** Large text overlays
- **Countdown (Circles):** Timer display

### Data Management
- **Import CSV/TXT:** Load contestant data files
- **Auto-fill:** Type names to auto-complete with age/state
- **Persistent Storage:** Data survives OBS sessions

### Control Interface
- **Compact Design:** Fits perfectly in OBS dock panels
- **Touch Friendly:** Mobile-optimized controls
- **Real-time Sync:** Instant updates between control and output

## Data Format

### CSV Format:
```csv
Name,Age,State
Amy Winehouse,29,Texas
Sarah Johnson,25,California
```

### TXT Format:
```
Amy Winehouse
29
Texas

Sarah Johnson
25
California
```

## Auto-fill Examples

- **Type "A" in title field** → Auto-fills "Amy Winehouse, 29"
- **Type "T" in subtitle field** → Auto-fills "Texas"

## Technical Details

- **Web Server:** Runs on `http://localhost:8080`
- **WebSocket:** Real-time communication on port 8081
- **Data Storage:** JSON format in plugin directory
- **Responsive:** Works on desktop and mobile devices

## Troubleshooting

### Control Interface Not Loading
1. Check if web server is running: `ps aux | grep python`
2. Verify URL: `http://localhost:8080/control.html`
3. Check browser console for errors

### Overlays Not Updating
1. Ensure WebSocket connection (port 8081)
2. Check OBS log for connection errors
3. Verify overlay configuration is being sent

### Data Not Loading
1. Check file format (CSV/TXT)
2. Verify file permissions
3. Check browser console for parsing errors

## Server Management

### Start Server:
```bash
cd ~/.config/obs-studio/plugins/overseer-control
python3 -m http.server 8080
```

### Stop Server:
```bash
pkill -f "python3 -m http.server 8080"
```

The control interface will work as long as the web server is running in the background.
