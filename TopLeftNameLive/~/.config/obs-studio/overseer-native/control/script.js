// OVERSEER CONTROL - Native OBS Integration
// Works completely offline with localStorage persistence

class OverseerControl {
    constructor() {
        this.overlays = [];
        this.contestants = [];
        this.nextId = 1;

        this.loadData();
        this.bindEvents();
        this.renderOverlays();

        // Update data info
        this.updateDataInfo();
    }

    loadData() {
        // Load contestants
        const contestantsData = localStorage.getItem('overseer-contestants');
        if (contestantsData) {
            try {
                this.contestants = JSON.parse(contestantsData);
            } catch (e) {
                console.error('Failed to load contestants:', e);
                this.contestants = [];
            }
        }

        // Load overlays
        const overlaysData = localStorage.getItem('overseer-overlays');
        if (overlaysData) {
            try {
                const overlayConfigs = JSON.parse(overlaysData);
                this.overlays = overlayConfigs.map(config => ({
                    id: config.id,
                    type: config.type,
                    name: this.getOverlayName(config.type),
                    position: config.position || 'UpLeft',
                    visible: config.visible !== false,
                    autoFillEnabled: config.autoFillEnabled !== false,
                    config: config.config || this.getDefaultConfig(config.type)
                }));

                // Update next ID
                const maxId = Math.max(...this.overlays.map(o => o.id), 0);
                this.nextId = maxId + 1;
            } catch (e) {
                console.error('Failed to load overlays:', e);
                this.overlays = [];
            }
        }
    }

    saveData() {
        // Save contestants
        localStorage.setItem('overseer-contestants', JSON.stringify(this.contestants));

        // Save overlays
        const overlayConfigs = this.overlays.map(overlay => ({
            id: overlay.id,
            type: overlay.type,
            position: overlay.position,
            visible: overlay.visible,
            autoFillEnabled: overlay.autoFillEnabled,
            config: overlay.config
        }));
        localStorage.setItem('overseer-overlays', JSON.stringify(overlayConfigs));
    }

    bindEvents() {
        document.getElementById('add-overlay-btn').addEventListener('click', () => {
            this.addOverlay();
        });

        document.getElementById('load-data-btn').addEventListener('click', () => {
            this.loadContestantData();
        });

        document.getElementById('clear-data-btn').addEventListener('click', () => {
            this.clearContestantData();
        });

        document.getElementById('export-data-btn').addEventListener('click', () => {
            this.exportContestantData();
        });

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

        const overlay = {
            id: this.nextId++,
            type: type,
            name: this.getOverlayName(type),
            position: 'UpLeft',
            visible: false,
            autoFillEnabled: true,
            config: this.getDefaultConfig(type)
        };

        this.overlays.push(overlay);
        this.saveData();
        this.renderOverlays();

        // Auto-scroll to new overlay
        setTimeout(() => {
            const newCard = document.querySelector(`[data-overlay-id="${overlay.id}"]`);
            if (newCard) {
                newCard.scrollIntoView({ behavior: 'smooth', block: 'center' });
            }
        }, 100);
    }

    removeOverlay(overlayId) {
        if (confirm('Remove this overlay?')) {
            this.overlays = this.overlays.filter(o => o.id !== overlayId);
            this.saveData();
            this.renderOverlays();
        }
    }

    updateOverlayPosition(overlayId, position) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (overlay) {
            overlay.position = position;
            this.saveData();
        }
    }

    updateOverlayAutoFill(overlayId, enabled) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (overlay) {
            overlay.autoFillEnabled = enabled;
            this.saveData();
        }
    }

    updateOverlayConfig(overlayId, config) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (overlay) {
            overlay.config = { ...overlay.config, ...config };
            this.saveData();
        }
    }

    setOverlayVisibility(overlayId, visible) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (overlay) {
            overlay.visible = visible;
            this.saveData();
            this.updateOverlayCard(overlayId);
        }
    }

    moveOverlay(overlayId, direction) {
        const index = this.overlays.findIndex(o => o.id === overlayId);
        if (index === -1) return;

        const newIndex = index + direction;
        if (newIndex >= 0 && newIndex < this.overlays.length) {
            [this.overlays[index], this.overlays[newIndex]] = [this.overlays[newIndex], this.overlays[index]];
            this.saveData();
            this.renderOverlays();
        }
    }

    copyOverlayPath(overlayId) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (!overlay) return;

        const basePath = window.location.href.replace('/control/index.html', '');
        const overlayPath = `${basePath}/overlays/${overlay.type}-output.html`;

        if (navigator.clipboard) {
            navigator.clipboard.writeText(overlayPath).then(() => {
                this.showNotification(`Path copied: ${overlay.type}-output.html`);
            }).catch(() => {
                this.showNotification(`Copy this path: ${overlayPath}`);
            });
        } else {
            this.showNotification(`Copy this path: ${overlayPath}`);
        }
    }

    loadContestantData() {
        const fileInput = document.getElementById('data-file');
        const file = fileInput.files[0];
        if (!file) return;

        const reader = new FileReader();
        reader.onload = (event) => {
            const content = event.target.result;
            let success = false;

            if (file.name.toLowerCase().endsWith('.csv')) {
                success = this.parseCSV(content);
            } else if (file.name.toLowerCase().endsWith('.txt')) {
                success = this.parseTXT(content);
            }

            if (success) {
                this.saveData();
                this.updateDataInfo();
                this.showNotification(`Loaded ${this.contestants.length} contestants from ${file.name}`);
            } else {
                this.showNotification('Failed to parse data file. Check format.');
            }
        };

        reader.readAsText(file);
    }

    parseCSV(content) {
        try {
            const lines = content.trim().split('\n');
            if (lines.length < 2) return false;

            this.contestants = [];

            // Skip header, parse data rows
            for (let i = 1; i < lines.length; i++) {
                const line = lines[i].trim();
                if (!line) continue;

                const parts = line.split(',');
                if (parts.length >= 3) {
                    this.contestants.push({
                        name: parts[0].trim(),
                        age: parseInt(parts[1].trim()) || 0,
                        state: parts[2].trim()
                    });
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
                const lines = entry.trim().split('\n').filter(line => line.trim());
                if (lines.length >= 3) {
                    this.contestants.push({
                        name: lines[0].trim(),
                        age: parseInt(lines[1].trim()) || 0,
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

    clearContestantData() {
        if (confirm('Clear all contestant data?')) {
            this.contestants = [];
            this.saveData();
            this.updateDataInfo();
            this.showNotification('Contestant data cleared');
        }
    }

    exportContestantData() {
        if (this.contestants.length === 0) {
            this.showNotification('No data to export');
            return;
        }

        let csv = 'Name,Age,State\n';
        this.contestants.forEach(contestant => {
            csv += `${contestant.name},${contestant.age},${contestant.state}\n`;
        });

        const blob = new Blob([csv], { type: 'text/csv' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'overseer-contestants-export.csv';
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);

        this.showNotification('Data exported as CSV');
    }

    getAutoFillSuggestion(text, isTitleField) {
        if (!text || text.length < 1 || this.contestants.length === 0) return '';

        const matches = this.contestants.filter(c =>
            c.name.toLowerCase().startsWith(text.toLowerCase())
        );

        if (matches.length > 0) {
            const match = matches[0];
            return isTitleField ? `${match.name}, ${match.age}` : match.state;
        }

        return '';
    }

    getOverlayName(type) {
        const names = {
            'logo': 'Logo',
            'lower-third': 'Lower Third',
            'live-bug': 'Live Bug',
            'big-lower-3rd': 'Big Lower Third',
            'countdown': 'Countdown'
        };
        return names[type] || 'Unknown';
    }

    getDefaultConfig(type) {
        const configs = {
            'logo': {
                text: 'SPONSOR',
                color: '#ffffff',
                bgColor: '#450313',
                fontSize: '24'
            },
            'lower-third': {
                title: 'Name, Age',
                subtitle: 'State',
                titleColor: '#ffffff',
                subtitleColor: '#ffffff',
                bgColor: '#450313'
            },
            'live-bug': {
                topText: 'LIVE from',
                bottomText: 'Location',
                topColor: '#ffffff',
                bottomColor: '#ffffff',
                bgColor: '#450313'
            },
            'big-lower-3rd': {
                title: 'Follow us',
                subtitle: 'Like and Subscribe!',
                titleColor: '#ffffff',
                subtitleColor: '#ffffff',
                bgColor1: '#450313',
                bgColor2: '#ff0035'
            },
            'countdown': {
                time: '5:00:00',
                color: '#ffffff',
                fontSize: '357'
            }
        };
        return configs[type] || {};
    }

    renderOverlays() {
        const grid = document.getElementById('overlays-grid');
        grid.innerHTML = '';

        this.overlays.forEach(overlay => {
            const card = this.createOverlayCard(overlay);
            grid.appendChild(card);
        });
    }

    createOverlayCard(overlay) {
        const card = document.createElement('div');
        card.className = 'overlay-card';
        card.setAttribute('data-overlay-id', overlay.id);

        const positionOptions = ['UpLeft', 'UpRight', 'BtLeft', 'BtRight', 'BtCenter'];

        card.innerHTML = `
            <div class="overlay-header">
                <div class="overlay-title">${overlay.name} #${overlay.id}</div>
                <div class="overlay-type">${overlay.type.replace('-', ' ').toUpperCase()}</div>
            </div>

            <div class="position-selector">
                <label>Position</label>
                <select class="position-select">
                    ${positionOptions.map(pos => `<option value="${pos}" ${pos === overlay.position ? 'selected' : ''}>${pos}</option>`).join('')}
                </select>
            </div>

            <div class="controls-grid">
                ${this.createControlGroups(overlay)}
            </div>

            <div class="autofill-toggle">
                <input type="checkbox" class="autofill-checkbox" ${overlay.autoFillEnabled ? 'checked' : ''}>
                <label>Auto-fill enabled</label>
            </div>

            <div class="action-buttons">
                <button class="action-btn show-btn">${overlay.visible ? 'HIDE' : 'SHOW'}</button>
                <button class="action-btn copy-path-btn">Copy Path</button>
                <button class="move-btn">◀</button>
                <button class="move-btn">▶</button>
                <button class="remove-btn">×</button>
            </div>
        `;

        // Bind events
        this.bindOverlayCardEvents(card, overlay.id);

        return card;
    }

    createControlGroups(overlay) {
        const config = overlay.config;

        switch (overlay.type) {
            case 'logo':
                return `
                    <div class="control-group">
                        <label>Text</label>
                        <input type="text" class="text-input" data-field="text" value="${config.text || ''}">
                    </div>
                    <div class="control-group">
                        <label>Text Color</label>
                        <input type="color" data-field="color" value="${config.color || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Background Color</label>
                        <input type="color" data-field="bgColor" value="${config.bgColor || '#450313'}">
                    </div>
                    <div class="control-group">
                        <label>Font Size</label>
                        <input type="range" min="12" max="72" data-field="fontSize" value="${config.fontSize || 24}">
                    </div>
                `;

            case 'lower-third':
                return `
                    <div class="control-group">
                        <label>Title</label>
                        <input type="text" class="text-input" data-field="title" value="${config.title || ''}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle</label>
                        <input type="text" class="text-input" data-field="subtitle" value="${config.subtitle || ''}">
                    </div>
                    <div class="control-group">
                        <label>Title Color</label>
                        <input type="color" data-field="titleColor" value="${config.titleColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle Color</label>
                        <input type="color" data-field="subtitleColor" value="${config.subtitleColor || '#ffffff'}">
                    </div>
                `;

            case 'live-bug':
                return `
                    <div class="control-group">
                        <label>Top Text</label>
                        <input type="text" class="text-input" data-field="topText" value="${config.topText || ''}">
                    </div>
                    <div class="control-group">
                        <label>Bottom Text</label>
                        <input type="text" class="text-input" data-field="bottomText" value="${config.bottomText || ''}">
                    </div>
                    <div class="control-group">
                        <label>Top Color</label>
                        <input type="color" data-field="topColor" value="${config.topColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Bottom Color</label>
                        <input type="color" data-field="bottomColor" value="${config.bottomColor || '#ffffff'}">
                    </div>
                `;

            case 'big-lower-3rd':
                return `
                    <div class="control-group">
                        <label>Title</label>
                        <input type="text" class="text-input" data-field="title" value="${config.title || ''}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle</label>
                        <input type="text" class="text-input" data-field="subtitle" value="${config.subtitle || ''}">
                    </div>
                    <div class="control-group">
                        <label>Title Color</label>
                        <input type="color" data-field="titleColor" value="${config.titleColor || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Subtitle Color</label>
                        <input type="color" data-field="subtitleColor" value="${config.subtitleColor || '#ffffff'}">
                    </div>
                `;

            case 'countdown':
                return `
                    <div class="control-group">
                        <label>Time (HH:MM:SS)</label>
                        <input type="text" data-field="time" value="${config.time || '5:00:00'}" placeholder="5:00:00">
                    </div>
                    <div class="control-group">
                        <label>Text Color</label>
                        <input type="color" data-field="color" value="${config.color || '#ffffff'}">
                    </div>
                    <div class="control-group">
                        <label>Font Size</label>
                        <input type="range" min="100" max="500" data-field="fontSize" value="${config.fontSize || 357}">
                    </div>
                `;

            default:
                return '<div class="control-group"><label>Unknown overlay type</label></div>';
        }
    }

    bindOverlayCardEvents(card, overlayId) {
        // Position change
        const positionSelect = card.querySelector('.position-select');
        positionSelect.addEventListener('change', (e) => {
            this.updateOverlayPosition(overlayId, e.target.value);
        });

        // Auto-fill toggle
        const autoFillCheckbox = card.querySelector('.autofill-checkbox');
        autoFillCheckbox.addEventListener('change', (e) => {
            this.updateOverlayAutoFill(overlayId, e.target.checked);
        });

        // Text input auto-fill
        const textInputs = card.querySelectorAll('.text-input');
        textInputs.forEach(input => {
            input.addEventListener('input', (e) => {
                const field = e.target.getAttribute('data-field');
                const isTitleField = field === 'title' || field === 'text' || field === 'topText';
                const overlay = this.overlays.find(o => o.id === overlayId);

                if (overlay && overlay.autoFillEnabled) {
                    const suggestion = this.getAutoFillSuggestion(e.target.value, isTitleField);
                    if (suggestion && suggestion !== e.target.value) {
                        e.target.value = suggestion;
                        this.updateOverlayConfig(overlayId, { [field]: suggestion });
                    }
                }
            });

            input.addEventListener('change', (e) => {
                const field = e.target.getAttribute('data-field');
                this.updateOverlayConfig(overlayId, { [field]: e.target.value });
            });
        });

        // Other input changes
        const inputs = card.querySelectorAll('input[data-field], select[data-field]');
        inputs.forEach(input => {
            if (!input.classList.contains('text-input')) {
                input.addEventListener('change', (e) => {
                    const field = e.target.getAttribute('data-field');
                    const value = e.target.type === 'range' ? e.target.value : e.target.value;
                    this.updateOverlayConfig(overlayId, { [field]: value });
                });
            }
        });

        // Action buttons
        const showBtn = card.querySelector('.show-btn');
        showBtn.addEventListener('click', () => {
            const overlay = this.overlays.find(o => o.id === overlayId);
            this.setOverlayVisibility(overlayId, !overlay.visible);
        });

        const copyBtn = card.querySelector('.copy-path-btn');
        copyBtn.addEventListener('click', () => {
            this.copyOverlayPath(overlayId);
        });

        const moveBtns = card.querySelectorAll('.move-btn');
        moveBtns[0].addEventListener('click', () => this.moveOverlay(overlayId, -1)); // Left
        moveBtns[1].addEventListener('click', () => this.moveOverlay(overlayId, 1));  // Right

        const removeBtn = card.querySelector('.remove-btn');
        removeBtn.addEventListener('click', () => {
            this.removeOverlay(overlayId);
        });
    }

    updateOverlayCard(overlayId) {
        const overlay = this.overlays.find(o => o.id === overlayId);
        if (!overlay) return;

        const card = document.querySelector(`[data-overlay-id="${overlayId}"]`);
        if (!card) return;

        const showBtn = card.querySelector('.show-btn');
        showBtn.textContent = overlay.visible ? 'HIDE' : 'SHOW';
        showBtn.className = `action-btn ${overlay.visible ? 'hide-btn' : 'show-btn'}`;
    }

    updateDataInfo() {
        const info = document.getElementById('data-info');
        info.textContent = `${this.contestants.length} contestants loaded`;
    }

    showNotification(message) {
        // Simple notification - could be enhanced with a proper toast system
        const notification = document.createElement('div');
        notification.style.cssText = `
            position: fixed;
            top: 20px;
            right: 20px;
            background: #3182ce;
            color: white;
            padding: 12px 16px;
            border-radius: 6px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            z-index: 1000;
            font-size: 14px;
            max-width: 300px;
        `;
        notification.textContent = message;

        document.body.appendChild(notification);

        setTimeout(() => {
            if (notification.parentNode) {
                notification.remove();
            }
        }, 3000);
    }
}

// Initialize when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    window.overseerControl = new OverseerControl();
});



