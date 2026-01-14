# Quick Start Guide

## For Immediate Setup on New Machine

### 1. Install Dependencies (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libobs-dev obs-studio-dev
```

### 2. Modify OBS Core (Required!)

Edit `libobs/obs-source.c`:
- Add `"void source_release(ptr source)"` to `source_signals[]` array
- Add `obs_source_dosignal(source, "source_release", "source_release");` in `obs_source_release()`
- Rebuild and reinstall OBS

### 3. Build Plugin

```bash
cd ~/Projects/overseer-obs-plugin-final
chmod +x build.sh
./build.sh
```

### 4. Verify

```bash
# Check plugin exists
ls ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so

# Check Qt symbols (should show "V" not "U")
nm -D ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so | grep OverseerDock

# Launch OBS
obs
```

### 5. In OBS

- Go to **View → Docks → "Overseer Control"**
- Dock should appear with overlay controls

## Active Project

**Use**: `~/Projects/overseer-obs-plugin-final/`

## Known Issue: Hardcoded Paths

Edit `~/Projects/overseer-obs-plugin-final/src/overseer-plugin.cpp`:
- Lines 203 and 493: Replace `/home/papaya/Projects/circle.png` with your path

## Full Documentation

See `PORTABLE_SETUP.md` for complete setup instructions and troubleshooting.

## Troubleshooting

- **Build fails**: Check dependencies installed correctly
- **Plugin won't load**: Verify OBS core modifications applied
- **OBS crashes**: Check OBS logs in `~/.config/obs-studio/logs/`

