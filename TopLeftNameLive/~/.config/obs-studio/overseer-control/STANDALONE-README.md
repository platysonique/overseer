# OVERSEER CONTROL - STANDALONE MODE

## 🎯 ONE-CLICK OVERLAY CONTROL FOR OBS

**No servers, no scripts, no hassle** - Just add Browser sources and you're ready to stream!

---

## 🚀 QUICK START (3 Steps)

### 1. Launch Control Interface
**Option A - Desktop Icon:**
- Look for "Overseer Control" in your applications menu
- Click to open the control interface in your browser

**Option B - Terminal:**
```bash
~/overseer-control-launcher.sh
```

### 2. Add Overlays in OBS
1. In the control interface, select overlay type (Logo, Lower Third, etc.)
2. Click **"Add Overlay"**
3. **Copy the file path** that appears
4. In OBS: **Sources → + → Browser**
5. **Paste the path** and set dimensions:
   - Width: `1920` (or your canvas width)
   - Height: `1080` (or your canvas height)

### 3. Load Contestant Data (Optional)
- Click **"Choose File"** in Data Management
- Select your `.csv` or `.txt` file
- Click **"Load Contestant Data"**
- Start typing in text fields for auto-fill!

---

## 📋 COMPLETE USAGE GUIDE

### Adding Overlay Types

#### Logo (Sponsor Bug)
- **File:** `overlays/logo/output.html`
- **Controls:** Sponsor text, colors, font size
- **Use:** Corner sponsor logos

#### Lower Third (On Air)
- **File:** `overlays/lower-third/output.html`
- **Controls:** Title, subtitle, colors
- **Use:** Name and location display

#### Live Bug
- **File:** `overlays/live-bug/output.html`
- **Controls:** Top/bottom text, colors
- **Use:** Live broadcast indicators

#### Big Lower Third
- **File:** `overlays/big-lower-3rd/output.html`
- **Controls:** Large title/subtitle
- **Use:** Major announcements

#### Countdown (Circles)
- **File:** `overlays/countdown/output.html`
- **Controls:** Time, colors, size
- **Use:** Event countdown timers

### Data File Formats

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

### Auto-Fill Examples

**Type "A" in a title field:**
- Auto-fills: "Amy Winehouse, 29"

**Type "T" in a subtitle field:**
- Auto-fills: "Texas"

---

## 🎮 CONTROL FEATURES

### ✅ Real-time Control
- **Show/Hide:** Toggle overlay visibility
- **Color Changes:** Live color updates
- **Text Updates:** Instant text changes
- **Auto-fill:** Smart text completion

### ✅ Data Management
- **CSV/TXT Import:** Load contestant lists
- **Auto-fill:** Type-ahead suggestions
- **Persistent Storage:** Data survives restarts
- **Export:** Save data as CSV

### ✅ OBS Integration
- **Browser Sources:** Native OBS compatibility
- **No Server Required:** Works offline
- **Local Files:** Direct file access
- **Multiple Overlays:** Unlimited instances

---

## 📁 FILE LOCATIONS

### Control Interface
```
~/.config/obs-studio/overseer-control/standalone-control.html
```

### Overlay Files
```
~/.config/obs-studio/overseer-control/overlays/
├── logo/output.html
├── lower-third/output.html
├── live-bug/output.html
├── big-lower-3rd/output.html
└── countdown/output.html
```

### Data Storage
- **Contestants:** Browser localStorage
- **Settings:** Browser localStorage
- **Paths:** Automatic path detection

---

## 🔧 TROUBLESHOOTING

### Control Interface Won't Open
```bash
# Manual launch
xdg-open ~/.config/obs-studio/overseer-control/standalone-control.html
```

### Overlays Not Updating
1. **Refresh Browser Source** in OBS after changes
2. **Check file permissions** on overlay HTML files
3. **Verify paths** are correct in OBS sources

### Auto-fill Not Working
1. **Check data loaded:** Look for contestant count
2. **Verify format:** CSV/TXT format correct
3. **Auto-fill enabled:** Check checkboxes in controls

### Colors Not Changing
1. **Refresh overlay** in OBS after color changes
2. **Check overlay type** supports that color setting
3. **Verify CSS** in overlay HTML matches control

---

## 🎯 ADVANCED USAGE

### Multiple Scenes
- Add the same overlay to multiple scenes
- Control from single interface
- Independent show/hide per scene

### Custom Branding
- Edit overlay HTML files directly
- Modify CSS for custom styling
- Add your own background images

### Live Streaming
- Use during live broadcasts
- Quick text updates for contestant names
- Real-time countdown management

---

## 📊 SYSTEM REQUIREMENTS

- **OBS Studio:** Any recent version
- **Web Browser:** Chrome, Firefox, or Edge
- **Operating System:** Linux (COSMIC desktop)
- **Storage:** ~10MB for all files
- **No Internet Required:** Works completely offline

---

## 🎉 READY TO STREAM!

Your beauty pageant overlay control system is now **completely standalone** and ready for production use.

**Just launch the control interface and start adding overlays to OBS!** 🎬✨

---

*For technical support or feature requests, check the main README.md file.*
