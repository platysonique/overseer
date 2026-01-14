#!/bin/bash
echo "Testing OBS WITHOUT plugin"
echo "6363" | sudo -S rm /usr/local/obs-plugins/64bit/overseer-plugin.so 2>/dev/null || true
timeout 10 pbmobs 2>&1 | head -30
echo "---"
echo "Testing OBS WITH plugin"  
echo "6363" | sudo -S cp ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/overseer-plugin.so /usr/local/obs-plugins/64bit/ 2>/dev/null || true
timeout 10 pbmobs 2>&1 | head -30
