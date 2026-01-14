// Overseer Control Panel JavaScript
class OverseerControl {
    constructor() {
        this.websocket = null;
        this.overlays = [];
        this.contestants = [];
        this.autoFillEnabled = {};
        this.connectWebSocket();
        this.bindEvents();
        this.updateStatus();
    }

    connectWebSocket() {
        this.websocket = new WebSocket('ws://localhost:8081');

        this.websocket.onopen = () => {
            console.log('Connected to Overseer server');
            document.getElementById('connection-status').textContent = 'Connected';
            document.getElementById('connection-status').style.background = '#2d7d32';
        };

        this.websocket.onmessage = (event) => {
            this.handleMessage(JSON.parse(event.data));
        };

        this.websocket.onclose = () => {
            console.log('Disconnected from Overseer server');
            document.getElementById('connection-status').textContent = 'Disconnected';
            document.getElementById('connection-status').style.background = '#d32f2f';

            // Attempt to reconnect after 3 seconds
            setTimeout(() => this.connectWebSocket(), 3000);
        };

        this.websocket.onerror = (error) => {
            console.error('WebSocket error:', error);
        };
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

        // File input change
        document.getElementById('data-file').addEventListener('change', (event) => {
            const file = event.target.files[0];
            if (file) {
                document.getElementById('load-data-btn').disabled = false;
            }
        });
    }

    addOverlay() {
        const typeSelect = document.getElementById('overlay-type');
        const type = typeSelect.value;

        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(JSON.stringify({
                action: 'addOverlay',
                type: type
            }));
        }
    }

    removeOverlay(index) {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(JSON.stringify({
                action: 'removeOverlay',
                index: index
            }));
        }
    }

    updateOverlayConfig(index, config) {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(JSON.stringify({
                action: 'updateOverlay',
                index: index,
                config: config
            }));
        }
    }

    moveOverlay(fromIndex, toIndex) {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(JSON.stringify({
                action: 'moveOverlay',
                fromIndex: fromIndex,
                toIndex: toIndex
            }));
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
                document.getElementById('data-status').textContent = 'Data loaded';
                document.getElementById('data-status').style.background = '#2d7d32';
                document.getElementById('contestant-count').textContent =
                    `${this.contestants.length} contestants loaded`;
            } else {
                alert('Failed to parse data file');
            }
        };

        reader.readAsText(file);
    }

    parseCSV(content) {
        try {
            const lines = content.split('\n');
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
            const entries = content.split('\n\n');
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
        this.contestants = [];
        document.getElementById('data-status').textContent = 'No data loaded';
        document.getElementById('data-status').style.background = '#666';
        document.getElementById('contestant-count').textContent = '0 contestants loaded';
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

    requestAutoFill(elementType, currentText, callback) {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            const message = {
                action: 'getAutoFill',
                elementType: elementType,
                currentText: currentText
            };

            // Store callback for response
            this.pendingCallbacks = this.pendingCallbacks || {};
            this.pendingCallbacks[elementType] = callback;

            this.websocket.send(JSON.stringify(message));
        } else {
            // Fallback to local auto-fill
            const suggestions = this.getAutoFillSuggestions(currentText, elementType);
            if (suggestions.length > 0) {
                callback(suggestions[0].displayText);
            }
        }
    }

    handleMessage(message) {
        const action = message.action;

        switch (action) {
            case 'overlayAdded':
                this.onOverlayAdded(message);
                break;
            case 'overlayRemoved':
                this.onOverlayRemoved(message);
                break;
            case 'overlayMoved':
                this.onOverlayMoved(message);
                break;
            case 'autoFillSuggestion':
                this.onAutoFillSuggestion(message);
                break;
        }
    }

    onOverlayAdded(message) {
        const overlay = {
            index: message.index,
            name: message.name,
            type: this.getOverlayTypeFromName(message.name),
            visible: false,
            config: this.getDefaultConfig(message.name)
        };

        this.overlays.push(overlay);
        this.renderOverlayPanel(overlay);
    }

    onOverlayRemoved(message) {
        const index = message.index;
        if (index >= 0 && index < this.overlays.length) {
            // Remove from DOM
            const panel = document.querySelector(`.overlay-panel[data-index="${index}"]`);
            if (panel) {
                panel.remove();
            }

            // Remove from array
            this.overlays.splice(index, 1);

            // Update indices
            this.updateOverlayIndices();
        }
    }

    onOverlayMoved(message) {
        const fromIndex = message.fromIndex;
        const toIndex = message.toIndex;

        // Reorder DOM elements
        const container = document.getElementById('overlays-container');
        const panels = Array.from(container.children);

        if (fromIndex < panels.length && toIndex < panels.length) {
            const movedPanel = panels[fromIndex];
            container.removeChild(movedPanel);
            container.insertBefore(movedPanel, panels[toIndex]);

            // Update overlay array
            const movedOverlay = this.overlays.splice(fromIndex, 1)[0];
            this.overlays.splice(toIndex, 0, movedOverlay);

            this.updateOverlayIndices();
        }
    }

    onAutoFillSuggestion(message) {
        const callback = this.pendingCallbacks && this.pendingCallbacks[message.elementType];
        if (callback && message.suggestion) {
            callback(message.suggestion);
        }
    }

    getOverlayTypeFromName(name) {
        if (name.includes('Logo')) return 'logo';
        if (name.includes('Lower Third')) return 'lower-third';
        if (name.includes('Live Bug')) return 'live-bug';
        if (name.includes('Big Lower')) return 'big-lower-3rd';
        if (name.includes('Countdown')) return 'countdown';
        return 'unknown';
    }

    getDefaultConfig(overlayName) {
        const type = this.getOverlayTypeFromName(overlayName);

        switch (type) {
            case 'logo':
                return {
                    sponsorText: 'SPONSOR',
                    textColor: '#ffffff',
                    bgColor: '#450313',
                    fontSize: 24,
                    fontFamily: 'Arial'
                };
            case 'lower-third':
                return {
                    titleText: 'Name, Age',
                    subtitleText: 'State',
                    titleColor: '#ffffff',
                    subtitleColor: '#ffffff',
                    bgColor: '#450313',
                    titleFontSize: 32,
                    subtitleFontSize: 24,
                    fontFamily: 'Arial'
                };
            case 'live-bug':
                return {
                    topText: 'LIVE from',
                    bottomText: 'Location',
                    topColor: '#ffffff',
                    bottomColor: '#ffffff',
                    bgColor: '#450313',
                    fontSize: 35,
                    fontFamily: 'Overpass'
                };
            case 'big-lower-3rd':
                return {
                    titleText: 'Follow us',
                    subtitleText: 'Like and Subscribe!',
                    titleColor: '#ffffff',
                    subtitleColor: '#ffffff',
                    bgColor1: '#450313',
                    bgColor2: '#ff0035',
                    titleFontSize: 40,
                    subtitleFontSize: 55,
                    fontFamily: 'Overpass'
                };
            case 'countdown':
                return {
                    time: '5:00:00',
                    color: '#ffffff',
                    fontSize: 357,
                    fontFamily: 'Open Sans',
                    bgColor: 'transparent'
                };
            default:
                return {};
        }
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
                    <button class="overlay-btn show-btn" onclick="overseerControl.showOverlay(${overlay.index})">Show</button>
                    <button class="overlay-btn hide-btn" onclick="overseerControl.hideOverlay(${overlay.index})">Hide</button>
                    <button class="overlay-btn move-btn" onclick="overseerControl.moveOverlayLeft(${overlay.index})">◄</button>
                    <button class="overlay-btn move-btn" onclick="overseerControl.moveOverlayRight(${overlay.index})">►</button>
                    <button class="overlay-btn remove-btn" onclick="overseerControl.removeOverlay(${overlay.index})">×</button>
                </div>
            </div>
            <div class="control-groups">
                ${this.renderControlGroups(type, config, overlay.index)}
            </div>
        `;

        container.appendChild(panel);

        // Bind auto-fill events
        this.bindAutoFillEvents(panel, overlay.index);
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
    }

    handleAutoFill(inputElement, elementType, overlayIndex) {
        const currentText = inputElement.value;
        if (currentText.length === 0) return;

        // Request auto-fill from server or use local data
        this.requestAutoFill(elementType, currentText, (suggestion) => {
            if (suggestion && suggestion !== currentText) {
                inputElement.value = suggestion;
                // Update config
                this.updateConfigFromInputs(overlayIndex);
            }
        });
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
        // Send show command to overlay
        this.updateOverlayConfig(index, { visible: true });
    }

    hideOverlay(index) {
        // Send hide command to overlay
        this.updateOverlayConfig(index, { visible: false });
    }

    moveOverlayLeft(index) {
        if (index > 0) {
            this.moveOverlay(index, index - 1);
        }
    }

    moveOverlayRight(index) {
        if (index < this.overlays.length - 1) {
            this.moveOverlay(index, index + 1);
        }
    }

    updateOverlayIndices() {
        const panels = document.querySelectorAll('.overlay-panel');
        panels.forEach((panel, newIndex) => {
            panel.setAttribute('data-index', newIndex);
            // Update button onclick handlers
            const buttons = panel.querySelectorAll('button[onclick]');
            buttons.forEach(button => {
                const onclick = button.getAttribute('onclick');
                const updatedOnclick = onclick.replace(/\d+/, newIndex.toString());
                button.setAttribute('onclick', updatedOnclick);
            });
        });
    }

    updateStatus() {
        // Update status every 5 seconds
        setInterval(() => {
            const status = this.websocket && this.websocket.readyState === WebSocket.OPEN
                ? 'Connected' : 'Disconnected';
            document.getElementById('connection-status').textContent = status;
            document.getElementById('connection-status').style.background =
                status === 'Connected' ? '#2d7d32' : '#d32f2f';
        }, 5000);
    }
}

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.overseerControl = new OverseerControl();
});
