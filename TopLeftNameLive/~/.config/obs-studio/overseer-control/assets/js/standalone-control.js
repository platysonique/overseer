// Overseer Standalone Control Interface
class OverseerStandalone {
    constructor() {
        this.overlays = [];
        this.contestants = [];
        this.loadStoredData();
        this.bindEvents();
        this.updatePathDisplay();
        this.updateContestantCount();
    }

    loadStoredData() {
        // Load contestants from localStorage
        const storedContestants = localStorage.getItem('overseer-contestants');
        if (storedContestants) {
            try {
                this.contestants = JSON.parse(storedContestants);
            } catch (e) {
                console.error('Failed to load contestant data:', e);
                this.contestants = [];
            }
        }

        // Load overlays from localStorage
        const storedOverlays = localStorage.getItem('overseer-overlays');
        if (storedOverlays) {
            try {
                const overlayConfigs = JSON.parse(storedOverlays);
                overlayConfigs.forEach(config => {
                    this.addOverlayFromConfig(config);
                });
            } catch (e) {
                console.error('Failed to load overlay data:', e);
            }
        }
    }

    saveData() {
        // Save contestants
        localStorage.setItem('overseer-contestants', JSON.stringify(this.contestants));

        // Save overlay configurations
        const overlayConfigs = this.overlays.map(overlay => ({
            index: overlay.index,
            name: overlay.name,
            type: overlay.type,
            config: overlay.config
        }));
        localStorage.setItem('overseer-overlays', JSON.stringify(overlayConfigs));
    }

    bindEvents() {
        // Add overlay button
        document.getElementById('add-overlay-btn').addEventListener('click', () => {
            this.addOverlay();
        });

        // Data file loading
        document.getElementById('load-data-btn').addEventListener('click', () => {
            this.loadDataFile();
        });

        document.getElementById('clear-data-btn').addEventListener('click', () => {
            this.clearData();
        });

        document.getElementById('export-data-btn').addEventListener('click', () => {
            this.exportData();
        });

        // File input change
        document.getElementById('data-file').addEventListener('change', (event) => {
            const file = event.target.files[0];
            if (file) {
                document.getElementById('load-data-btn').disabled = false;
            }
        });
    }

    updatePathDisplay() {
        // Get the current directory path for OBS browser sources
        const currentPath = window.location.href;
        const basePath = currentPath.substring(0, currentPath.lastIndexOf('/'));
        document.getElementById('current-path').textContent = basePath + '/overlays/[overlay-type]/output.html';
    }

    addOverlay() {
        const typeSelect = document.getElementById('overlay-type');
        const type = typeSelect.value;

        const overlay = {
            index: Date.now(), // Use timestamp as unique ID
            name: this.getOverlayName(type),
            type: type,
            config: this.getDefaultConfig(type)
        };

        this.overlays.push(overlay);
        this.renderOverlayPanel(overlay);
        this.saveData();

        // Show the file path for this overlay
        this.showOverlayPath(overlay);
    }

    addOverlayFromConfig(config) {
        const overlay = {
            index: config.index,
            name: config.name,
            type: config.type,
            config: config.config
        };

        this.overlays.push(overlay);
        this.renderOverlayPanel(overlay);
    }

    removeOverlay(index) {
        // Remove from DOM
        const panel = document.querySelector(`.overlay-panel[data-index="${index}"]`);
        if (panel) {
            panel.remove();
        }

        // Remove from array
        this.overlays = this.overlays.filter(o => o.index !== index);

        // Update indices
        this.saveData();
    }

    updateOverlayConfig(index, config) {
        const overlay = this.overlays.find(o => o.index === index);
        if (overlay) {
            overlay.config = config;

            // Store config in localStorage for this overlay type
            localStorage.setItem(`overseer-overlay-${index}`, JSON.stringify(config));

            this.saveData();
        }
    }

    showOverlayPath(overlay) {
        const basePath = window.location.href.substring(0, window.location.href.lastIndexOf('/'));
        const overlayPath = `${basePath}/overlays/${overlay.type}/output.html`;

        // Copy to clipboard if possible
        if (navigator.clipboard) {
            navigator.clipboard.writeText(overlayPath).then(() => {
                alert(`Overlay path copied to clipboard:\n${overlayPath}\n\nAdd this as a Browser source in OBS!`);
            });
        } else {
            alert(`Copy this path for OBS Browser source:\n${overlayPath}`);
        }
    }

    loadDataFile() {
        const fileInput = document.getElementById('data-file');
        const file = fileInput.files[0];

        if (!file) return;

        const reader = new FileReader();

        reader.onload = (event) => {
            const content = event.target.result;
            let success = false;

            if (file.name.endsWith('.csv')) {
                success = this.parseCSV(content);
            } else if (file.name.endsWith('.txt')) {
                success = this.parseTXT(content);
            }

            if (success) {
                this.saveData();
                this.updateContestantCount();
                alert(`Loaded ${this.contestants.length} contestants from ${file.name}`);
            } else {
                alert('Failed to parse data file. Check format and try again.');
            }
        };

        reader.readAsText(file);
    }

    parseCSV(content) {
        try {
            const lines = content.trim().split('\n');
            this.contestants = [];

            // Skip header row
            for (let i = 1; i < lines.length; i++) {
                const line = lines[i].trim();
                if (line) {
                    const parts = line.split(',');
                    if (parts.length >= 3) {
                        this.contestants.push({
                            name: parts[0].trim(),
                            age: parseInt(parts[1].trim()),
                            state: parts[2].trim()
                        });
                    }
                }
            }

            return true;
        } catch (error) {
            console.error('CSV parsing error:', error);
            return false;
        }
    }

    parseTXT(content) {
        try {
            const entries = content.trim().split('\n\n');
            this.contestants = [];

            for (const entry of entries) {
                const lines = entry.trim().split('\n');
                if (lines.length >= 3) {
                    this.contestants.push({
                        name: lines[0].trim(),
                        age: parseInt(lines[1].trim()),
                        state: lines[2].trim()
                    });
                }
            }

            return true;
        } catch (error) {
            console.error('TXT parsing error:', error);
            return false;
        }
    }

    clearData() {
        if (confirm('Are you sure you want to clear all contestant data?')) {
            this.contestants = [];
            this.saveData();
            this.updateContestantCount();
        }
    }

    exportData() {
        if (this.contestants.length === 0) {
            alert('No contestant data to export.');
            return;
        }

        // Create CSV content
        let csvContent = 'Name,Age,State\n';
        this.contestants.forEach(contestant => {
            csvContent += `${contestant.name},${contestant.age},${contestant.state}\n`;
        });

        // Create and download file
        const blob = new Blob([csvContent], { type: 'text/csv' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'contestants-export.csv';
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
    }

    getAutoFillSuggestions(text, elementType) {
        if (!text || this.contestants.length === 0) return [];

        const matches = this.contestants.filter(contestant =>
            contestant.name.toLowerCase().startsWith(text.toLowerCase())
        ).slice(0, 5);

        return matches.map(match => ({
            fullName: match.name,
            displayText: elementType === 'title' || elementType === 'name'
                ? `${match.name}, ${match.age}`
                : match.state,
            contestant: match
        }));
    }

    getOverlayName(type) {
        const names = {
            'logo': 'Logo Overlay',
            'lower-third': 'Lower Third',
            'live-bug': 'Live Bug',
            'big-lower-3rd': 'Big Lower Third',
            'countdown': 'Countdown'
        };
        return names[type] || 'Unknown Overlay';
    }

    getDefaultConfig(type) {
        const configs = {
            'logo': {
                sponsorText: 'SPONSOR',
                textColor: '#ffffff',
                bgColor: '#450313',
                fontSize: 24,
                fontFamily: 'Arial'
            },
            'lower-third': {
                titleText: 'Name, Age',
                subtitleText: 'State',
                titleColor: '#ffffff',
                subtitleColor: '#ffffff',
                bgColor: '#450313',
                titleFontSize: 32,
                subtitleFontSize: 24,
                fontFamily: 'Arial'
            },
            'live-bug': {
                topText: 'LIVE from',
                bottomText: 'Location',
                topColor: '#ffffff',
                bottomColor: '#ffffff',
                bgColor: '#450313',
                fontSize: 35,
                fontFamily: 'Overpass'
            },
            'big-lower-3rd': {
                titleText: 'Follow us',
                subtitleText: 'Like and Subscribe!',
                titleColor: '#ffffff',
                subtitleColor: '#ffffff',
                bgColor1: '#450313',
                bgColor2: '#ff0035',
                titleFontSize: 40,
                subtitleFontSize: 55,
                fontFamily: 'Overpass'
            },
            'countdown': {
                time: '5:00:00',
                color: '#ffffff',
                fontSize: 357,
                fontFamily: 'Open Sans',
                bgColor: 'transparent'
            }
        };
        return configs[type] || {};
    }

    renderOverlayPanel(overlay) {
        const container = document.getElementById('overlays-container');
        const panel = document.createElement('div');
        panel.className = 'overlay-panel';
        panel.setAttribute('data-index', overlay.index);

        const config = overlay.config;
        const type = overlay.type;

        panel.innerHTML = `
            <div class="overlay-header">
                <div class="overlay-title">${overlay.name}</div>
                <div class="overlay-controls">
                    <button class="overlay-btn show-btn" onclick="overseerStandalone.showOverlay(${overlay.index})">Show</button>
                    <button class="overlay-btn hide-btn" onclick="overseerStandalone.hideOverlay(${overlay.index})">Hide</button>
                    <button class="overlay-btn remove-btn" onclick="overseerStandalone.removeOverlay(${overlay.index})">×</button>
                </div>
            </div>
            <div class="control-groups">
                ${this.renderControlGroups(type, config, overlay.index)}
            </div>
            <div style="margin-top: 10px; padding: 8px; background: #2a2a2a; border-radius: 4px; font-size: 12px;">
                <strong>OBS Path:</strong>
                <code style="word-break: break-all;">${this.getOverlayPath(overlay)}</code>
                <button onclick="overseerStandalone.copyOverlayPath('${this.getOverlayPath(overlay)}')" style="margin-left: 8px; padding: 2px 6px; font-size: 11px;">Copy</button>
            </div>
        `;

        container.appendChild(panel);

        // Bind auto-fill events
        this.bindAutoFillEvents(panel, overlay.index);
    }

    getOverlayPath(overlay) {
        const basePath = window.location.href.substring(0, window.location.href.lastIndexOf('/'));
        return `${basePath}/overlays/${overlay.type}/output.html`;
    }

    copyOverlayPath(path) {
        if (navigator.clipboard) {
            navigator.clipboard.writeText(path).then(() => {
                // Show temporary feedback
                const btn = event.target;
                const originalText = btn.textContent;
                btn.textContent = 'Copied!';
                setTimeout(() => btn.textContent = originalText, 1000);
            });
        } else {
            alert(`Copy this path:\n${path}`);
        }
    }

    renderControlGroups(type, config, overlayIndex) {
        switch (type) {
            case 'logo':
                return `
                    <div class="control-group">
                        <label>Sponsor Text</label>
                        <input type="text" data-element="sponsorText" value="${config.sponsorText || ''}" data-autofill="name">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-sponsor-${overlayIndex}" checked>
                            <label for="autofill-sponsor-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Text Color</label>
                        <input type="color" data-element="textColor" value="${config.textColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Background Color</label>
                        <input type="color" data-element="bgColor" value="${config.bgColor || '#450313'}">
                    </div>
                    <div class="control-group">
                        <label>Font Size</label>
                        <input type="range" min="12" max="72" data-element="fontSize" value="${config.fontSize || 24}">
                    </div>
                `;

            case 'lower-third':
                return `
                    <div class="control-group">
                        <label>Title Text</label>
                        <input type="text" data-element="titleText" value="${config.titleText || ''}" data-autofill="title">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-title-${overlayIndex}" checked>
                            <label for="autofill-title-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Subtitle Text</label>
                        <input type="text" data-element="subtitleText" value="${config.subtitleText || ''}" data-autofill="subtitle">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-subtitle-${overlayIndex}" checked>
                            <label for="autofill-subtitle-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Title Color</label>
                        <input type="color" data-element="titleColor" value="${config.titleColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle Color</label>
                        <input type="color" data-element="subtitleColor" value="${config.subtitleColor || '#ffffff'}">
                    </div>
                `;

            case 'live-bug':
                return `
                    <div class="control-group">
                        <label>Top Text</label>
                        <input type="text" data-element="topText" value="${config.topText || 'LIVE from'}" data-autofill="title">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-top-${overlayIndex}" checked>
                            <label for="autofill-top-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Bottom Text</label>
                        <input type="text" data-element="bottomText" value="${config.bottomText || ''}" data-autofill="subtitle">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-bottom-${overlayIndex}" checked>
                            <label for="autofill-bottom-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Top Color</label>
                        <input type="color" data-element="topColor" value="${config.topColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Bottom Color</label>
                        <input type="color" data-element="bottomColor" value="${config.bottomColor || '#ffffff'}">
                    </div>
                `;

            case 'big-lower-3rd':
                return `
                    <div class="control-group">
                        <label>Title Text</label>
                        <input type="text" data-element="titleText" value="${config.titleText || 'Follow us'}" data-autofill="title">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-bigtitle-${overlayIndex}" checked>
                            <label for="autofill-bigtitle-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Subtitle Text</label>
                        <input type="text" data-element="subtitleText" value="${config.subtitleText || 'Like and Subscribe!'}" data-autofill="subtitle">
                        <div class="autofill-control">
                            <input type="checkbox" id="autofill-bigsubtitle-${overlayIndex}" checked>
                            <label for="autofill-bigsubtitle-${overlayIndex}">Auto-fill</label>
                        </div>
                    </div>
                    <div class="control-group">
                        <label>Title Color</label>
                        <input type="color" data-element="titleColor" value="${config.titleColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle Color</label>
                        <input type="color" data-element="subtitleColor" value="${config.subtitleColor || '#ffffff'}">
                    </div>
                `;

            case 'countdown':
                return `
                    <div class="control-group">
                        <label>Time (HH:MM:SS)</label>
                        <input type="text" data-element="time" value="${config.time || '5:00:00'}" placeholder="5:00:00">
                    </div>
                    <div class="control-group">
                        <label>Text Color</label>
                        <input type="color" data-element="color" value="${config.color || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Font Size</label>
                        <input type="range" min="100" max="500" data-element="fontSize" value="${config.fontSize || 357}">
                    </div>
                    <div class="control-group">
                        <label>Background Color</label>
                        <input type="color" data-element="bgColor" value="${config.bgColor || 'transparent'}">
                    </div>
                `;

            default:
                return '<div class="control-group"><p>Unknown overlay type</p></div>';
        }
    }

    bindAutoFillEvents(panel, overlayIndex) {
        const inputs = panel.querySelectorAll('input[data-autofill]');
        inputs.forEach(input => {
            input.addEventListener('input', (event) => {
                const elementType = event.target.getAttribute('data-autofill');
                const checkbox = panel.querySelector(`#autofill-${elementType}-${overlayIndex}`);

                if (checkbox && checkbox.checked) {
                    this.handleAutoFill(event.target, elementType, overlayIndex);
                }
            });
        });

        // Bind config change events
        const configInputs = panel.querySelectorAll('input[data-element], select[data-element]');
        configInputs.forEach(input => {
            input.addEventListener('change', () => {
                this.updateConfigFromInputs(overlayIndex);
            });
        });
    }

    handleAutoFill(inputElement, elementType, overlayIndex) {
        const currentText = inputElement.value;
        if (currentText.length === 0) return;

        const suggestions = this.getAutoFillSuggestions(currentText, elementType);
        if (suggestions.length > 0) {
            inputElement.value = suggestions[0].displayText;
            this.updateConfigFromInputs(overlayIndex);
        }
    }

    updateConfigFromInputs(overlayIndex) {
        const panel = document.querySelector(`.overlay-panel[data-index="${overlayIndex}"]`);
        if (!panel) return;

        const config = {};
        const inputs = panel.querySelectorAll('input[data-element], select[data-element]');

        inputs.forEach(input => {
            const element = input.getAttribute('data-element');
            if (input.type === 'checkbox') {
                config[element] = input.checked;
            } else {
                config[element] = input.value;
            }
        });

        this.updateOverlayConfig(overlayIndex, config);
    }

    showOverlay(index) {
        // Store show state
        localStorage.setItem(`overseer-overlay-${index}-visible`, 'true');
        alert('Overlay marked as visible. Refresh the overlay Browser source in OBS to see changes.');
    }

    hideOverlay(index) {
        // Store hide state
        localStorage.setItem(`overseer-overlay-${index}-visible`, 'false');
        alert('Overlay marked as hidden. Refresh the overlay Browser source in OBS to see changes.');
    }

    updateContestantCount() {
        document.getElementById('contestant-count').textContent =
            `${this.contestants.length} contestants loaded`;
    }
}

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.overseerStandalone = new OverseerStandalone();
});
