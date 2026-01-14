#!/bin/bash
# Overseer Plugin Test Script

echo "Testing Overseer Plugin..."

# Check if plugin file exists
if [ ! -f ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer.so ]; then
    echo "❌ Plugin file not found!"
    exit 1
fi

echo "✅ Plugin file found"

# Check if plugin is executable
if [ ! -x ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer.so ]; then
    echo "❌ Plugin file not executable!"
    exit 1
fi

echo "✅ Plugin file is executable"

# Check web interface files
if [ ! -f ~/.config/obs-studio/plugins/overseer-plugin/data/web/control.html ]; then
    echo "❌ Control interface not found!"
    exit 1
fi

echo "✅ Control interface found"

# Check overlay templates
OVERLAYS=("logo" "lower-third" "live-bug" "big-lower-3rd" "countdown")
for overlay in "${OVERLAYS[@]}"; do
    if [ ! -d ~/.config/obs-studio/plugins/overseer-plugin/data/overlays/$overlay ]; then
        echo "❌ Overlay template missing: $overlay"
        exit 1
    fi
done

echo "✅ All overlay templates found"

# Check sample data files
if [ ! -f ~/.config/obs-studio/plugins/overseer-plugin/data/sample-contestants.csv ]; then
    echo "❌ Sample CSV file missing!"
    exit 1
fi

if [ ! -f ~/.config/obs-studio/plugins/overseer-plugin/data/sample-contestants.txt ]; then
    echo "❌ Sample TXT file missing!"
    exit 1
fi

echo "✅ Sample data files found"

echo ""
echo "🎉 Overseer Plugin verification completed successfully!"
echo ""
echo "Next steps:"
echo "1. Start OBS Studio"
echo "2. Add 'Overseer Plugin' as a source"
echo "3. Dock the control interface"
echo "4. Load sample contestant data"
echo "5. Add and customize overlays"
echo ""
echo "Control interface: http://localhost:8080/control.html"
echo "WebSocket server: localhost:8081"
