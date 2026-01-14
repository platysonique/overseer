#include "overseer-dock.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QDebug>
#include <QDateTime>
#include <QCompleter>

OverseerDock::OverseerDock(QWidget* parent)
    : QDockWidget("Overseer Control", parent),
      dataIngestor(new DataIngestor()),
      settings(new QSettings("PombombMedia", "OverseerPlugin")),
      countdownTimer(new QTimer(this))
{
    createUI();
    loadSettings();

    // Setup countdown timer
    connect(countdownTimer, &QTimer::timeout, this, &OverseerDock::onCountdownChanged);
    countdownTimer->start(1000); // Update every second
}

OverseerDock::~OverseerDock() {
    saveSettings();
    qDeleteAll(overlays);
    delete dataIngestor;
    delete settings;
}

void OverseerDock::createUI() {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* mainWidget = new QWidget();
    mainLayout = new QVBoxLayout(mainWidget);

    // Create controls section
    QWidget* controlsWidget = new QWidget();
    QHBoxLayout* controlsLayout = new QHBoxLayout(controlsWidget);

    // Overlay type selector
    overlayTypeSelect = new QComboBox();
    overlayTypeSelect->addItem("Select Type...", -1);
    overlayTypeSelect->addItem("Logo", static_cast<int>(OverlayType::LOGO));
    overlayTypeSelect->addItem("Lower Third", static_cast<int>(OverlayType::LOWER_THIRD));
    overlayTypeSelect->addItem("Live Bug", static_cast<int>(OverlayType::LIVE_BUG));
    overlayTypeSelect->addItem("Big Lower 3rd", static_cast<int>(OverlayType::BIG_LOWER_3RD));
    overlayTypeSelect->addItem("Countdown", static_cast<int>(OverlayType::COUNTDOWN));
    controlsLayout->addWidget(new QLabel("Type:"));
    controlsLayout->addWidget(overlayTypeSelect);

    // Add overlay button
    addOverlayBtn = new QPushButton("Add Overlay");
    addOverlayBtn->setEnabled(false);
    controlsLayout->addWidget(addOverlayBtn);

    // Ingest data button
    ingestDataBtn = new QPushButton("Ingest Data");
    controlsLayout->addWidget(ingestDataBtn);

    mainLayout->addWidget(controlsWidget);

    // Create scrollable area for overlay controls
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scrollContent = new QWidget();
    overlayLayout = new QVBoxLayout(scrollContent);
    overlayLayout->addStretch(); // Push controls to top

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    setWidget(mainWidget);

    // Connect signals
    connect(overlayTypeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OverseerDock::onOverlayTypeChanged);
    connect(addOverlayBtn, &QPushButton::clicked, this, &OverseerDock::onAddOverlayClicked);
    connect(ingestDataBtn, &QPushButton::clicked, this, &OverseerDock::onIngestDataClicked);
}

void OverseerDock::onOverlayTypeChanged(int index) {
    bool enabled = (overlayTypeSelect->currentData().toInt() != -1);
    addOverlayBtn->setEnabled(enabled);
}

void OverseerDock::onAddOverlayClicked() {
    int typeIndex = overlayTypeSelect->currentData().toInt();
    if (typeIndex == -1) return;

    OverlayType type = static_cast<OverlayType>(typeIndex);
    addOverlay(type);

    // Reset selector
    overlayTypeSelect->setCurrentIndex(0);
}

void OverseerDock::addOverlay(OverlayType type) {
    OverseerOverlay* overlay = new OverseerOverlay(getUniqueOverlayName(type), type);
    if (!overlay->createOBSSource()) {
        QMessageBox::critical(this, "Error", "Failed to create overlay source");
        delete overlay;
        return;
    }

    overlays.append(overlay);
    createOverlayControls(overlay);
    saveSettings();

    qDebug() << "Added overlay:" << overlay->getParameters()->overlayName;
}

void OverseerDock::createOverlayControls(OverseerOverlay* overlay) {
    OverlayParameters* params = overlay->getParameters();

    // Create group box for this overlay
    QGroupBox* groupBox = new QGroupBox(params->overlayName);
    QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);

    // Common controls
    groupLayout->addWidget(createPositionControls(params));
    groupLayout->addWidget(createScaleControls(params));
    groupLayout->addWidget(createVisibilityControls());

    // Type-specific controls
    QWidget* typeControls = createTypeSpecificControls(params->type, params);
    if (typeControls) {
        groupLayout->addWidget(typeControls);
    }

    // Remove button
    QPushButton* removeBtn = new QPushButton("Remove Overlay");
    removeBtn->setProperty("overlayId", generateOverlayId());
    connect(removeBtn, &QPushButton::clicked, this, &OverseerDock::onRemoveOverlayClicked);
    groupLayout->addWidget(removeBtn);

    // Add to scroll area
    overlayLayout->insertWidget(overlayLayout->count() - 1, groupBox); // Insert before stretch
}

QWidget* OverseerDock::createPositionControls(OverlayParameters* params) {
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);

    layout->addWidget(new QLabel("Position:"));

    QSpinBox* xSpin = new QSpinBox();
    xSpin->setRange(0, 1920);
    xSpin->setValue(params->mainCanvasX);
    xSpin->setProperty("overlayId", generateOverlayId());
    connect(xSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OverseerDock::onPositionXChanged);
    layout->addWidget(new QLabel("X:"));
    layout->addWidget(xSpin);

    QSpinBox* ySpin = new QSpinBox();
    ySpin->setRange(0, 1080);
    ySpin->setValue(params->mainCanvasY);
    ySpin->setProperty("overlayId", generateOverlayId());
    connect(ySpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OverseerDock::onPositionYChanged);
    layout->addWidget(new QLabel("Y:"));
    layout->addWidget(ySpin);

    layout->addStretch();
    return widget;
}

QWidget* OverseerDock::createScaleControls(OverlayParameters* params) {
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);

    layout->addWidget(new QLabel("Scale:"));

    QDoubleSpinBox* scaleSpin = new QDoubleSpinBox();
    scaleSpin->setRange(0.1, 5.0);
    scaleSpin->setSingleStep(0.1);
    scaleSpin->setValue(params->scale);
    scaleSpin->setProperty("overlayId", generateOverlayId());
    connect(scaleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &OverseerDock::onScaleChanged);
    layout->addWidget(scaleSpin);

    layout->addStretch();
    return widget;
}

QWidget* OverseerDock::createVisibilityControls() {
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);

    QCheckBox* visibleCheck = new QCheckBox("Visible");
    visibleCheck->setChecked(true);
    visibleCheck->setProperty("overlayId", generateOverlayId());
    connect(visibleCheck, &QCheckBox::toggled, this, &OverseerDock::onVisibilityChanged);
    layout->addWidget(visibleCheck);

    layout->addStretch();
    return widget;
}

QWidget* OverseerDock::createTypeSpecificControls(OverlayType type, OverlayParameters* params) {
    switch (type) {
        case OverlayType::LOGO:
            return createLogoControls(dynamic_cast<LogoParameters*>(params));
        case OverlayType::LOWER_THIRD:
            return createLowerThirdControls(dynamic_cast<LowerThirdParameters*>(params));
        case OverlayType::LIVE_BUG:
            return createLiveBugControls(dynamic_cast<LiveBugParameters*>(params));
        case OverlayType::BIG_LOWER_3RD:
            return createBigLower3rdControls(dynamic_cast<BigLower3rdParameters*>(params));
        case OverlayType::COUNTDOWN:
            return createCountdownControls(dynamic_cast<CountdownParameters*>(params));
        default:
            return nullptr;
    }
}

QWidget* OverseerDock::createLowerThirdControls(LowerThirdParameters* params) {
    QWidget* w = new QWidget();
    QFormLayout* l = new QFormLayout(w);
    if (!params) return w;
    layout->addWidget(logoBtn);

    // Text inputs with auto-fill
    QLineEdit* topTextEdit = new QLineEdit(params->topText);
    topTextEdit->setPlaceholderText("Top Text");
    topTextEdit->setProperty("overlayId", generateOverlayId());
    topTextEdit->setProperty("textType", "topText");
    if (dataIngestor->hasData()) {
        QCompleter* completer = new QCompleter(dataIngestor->getFieldValues("name"));
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        topTextEdit->setCompleter(completer);
    }
    connect(topTextEdit, &QLineEdit::textChanged, this, &OverseerDock::onTextChanged);
    layout->addWidget(topTextEdit);

    QLineEdit* bottomTextEdit = new QLineEdit(params->bottomText);
    bottomTextEdit->setPlaceholderText("Bottom Text");
    bottomTextEdit->setProperty("overlayId", generateOverlayId());
    bottomTextEdit->setProperty("textType", "bottomText");
    connect(bottomTextEdit, &QLineEdit::textChanged, this, &OverseerDock::onTextChanged);
    layout->addWidget(bottomTextEdit);

    // Font controls
    QHBoxLayout* fontLayout = new QHBoxLayout();
    fontLayout->addWidget(new QLabel("Font Size:"));
    QSpinBox* fontSizeSpin = new QSpinBox();
    fontSizeSpin->setRange(8, 120);
    fontSizeSpin->setValue(params->fontSize);
    fontSizeSpin->setProperty("overlayId", generateOverlayId());
    connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OverseerDock::onFontChanged);
    fontLayout->addWidget(fontSizeSpin);
    layout->addLayout(fontLayout);

    // Color controls
    QPushButton* textColorBtn = createColorPickerButton("Text Color", params->textColor);
    textColorBtn->setProperty("colorType", "textColor");
    layout->addWidget(textColorBtn);

    QPushButton* bgColorBtn = createColorPickerButton("Background Color", params->backgroundColor);
    bgColorBtn->setProperty("colorType", "backgroundColor");
    layout->addWidget(bgColorBtn);

    return widget;
}

QWidget* OverseerDock::createLogoControls(OverseerOverlay* overlay) {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);

    auto* p = dynamic_cast<LogoParameters*>(overlay->getParameters());
    if (!p) return widget;

    QPushButton* imageBtn = new QPushButton("Select Image");
    layout->addWidget(imageBtn);
    connect(imageBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<LogoParameters*>(overlay->getParameters());
                if (!p) return;
                QString fp = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.webp)");
                if (fp.isEmpty()) return;
                p->imagePath = fp;
                overlay->updateChildSourceProperty("LogoImage", "file", fp);
            });

    return widget;
}

QWidget* OverseerDock::createLiveBugControls(OverseerOverlay* overlay) {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);

    auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());
    if (!p) return widget;

    QLineEdit* textEdit = new QLineEdit(p->text);
    layout->addWidget(textEdit);
    connect(textEdit, &QLineEdit::textChanged, this,
            [overlay](const QString& t) {
                auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());
                if (p) {
                    p->text = t;
                    overlay->updateChildSourceProperty("LiveText", "text", t);
                }
            });

    QPushButton* textColorBtn = new QPushButton("Pick Text Color");
    layout->addWidget(textColorBtn);
    connect(textColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->textColor, this, "Select Text Color");
                if (!c.isValid()) return;
                p->textColor = c;
            });

    QPushButton* bgColorBtn = new QPushButton("Pick BG Color");
    layout->addWidget(bgColorBtn);
    connect(bgColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<LiveBugParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->backgroundColor, this, "Select Background Color");
                if (!c.isValid()) return;
                p->backgroundColor = c;
                uint32_t argb =
                    (uint32_t(c.blue()) |
                     (uint32_t(c.green()) << 8) |
                     (uint32_t(c.red()) << 16) |
                     (uint32_t(c.alpha()) << 24));
                overlay->updateChildSourceProperty("Background", "color", QVariant::fromValue((int64_t)argb));
            });

    return widget;
}

QWidget* OverseerDock::createBigLower3rdControls(OverseerOverlay* overlay) {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);

    auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());
    if (!p) return widget;

    QLineEdit* titleEdit = new QLineEdit(p->titleText);
    titleEdit->setPlaceholderText("Title Text");
    titleEdit->setProperty("overlayId", generateOverlayId());
    titleEdit->setProperty("textType", "titleText");
    connect(titleEdit, &QLineEdit::textChanged, this, &OverseerDock::onTextChanged);
    layout->addWidget(titleEdit);

    QLineEdit* subtitleEdit = new QLineEdit(p->subtitleText);
    subtitleEdit->setPlaceholderText("Subtitle Text");
    layout->addWidget(subtitleEdit);
    connect(subtitleEdit, &QLineEdit::textChanged, this,
            [overlay](const QString& t) {
                auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());
                if (p) {
                    p->subtitleText = t;
                    overlay->updateChildSourceProperty("SubtitleText", "text", t);
                }
            });

    QPushButton* textColorBtn = new QPushButton("Pick Text Color");
    layout->addWidget(textColorBtn);
    connect(textColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->titleTextColor, this, "Select Text Color");
                if (!c.isValid()) return;
                p->titleTextColor = c;
                p->subtitleTextColor = c; // Use same color for both
            });

    QPushButton* bgColorBtn = new QPushButton("Pick BG Color");
    layout->addWidget(bgColorBtn);
    connect(bgColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<BigLower3rdParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->backgroundColor, this, "Select Background Color");
                if (!c.isValid()) return;
                p->backgroundColor = c;
                uint32_t argb =
                    (uint32_t(c.blue()) |
                     (uint32_t(c.green()) << 8) |
                     (uint32_t(c.red()) << 16) |
                     (uint32_t(c.alpha()) << 24));
                overlay->updateChildSourceProperty("Background", "color", QVariant::fromValue((int64_t)argb));
            });

    return widget;
}

QWidget* OverseerDock::createCountdownControls(OverseerOverlay* overlay) {
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);

    auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());
    if (!p) return widget;

    QHBoxLayout* timeLayout = new QHBoxLayout();

    QSpinBox* hoursSpin = new QSpinBox();
    hoursSpin->setRange(0, 23);
    hoursSpin->setValue(p->hours);
    hoursSpin->setProperty("overlayId", generateOverlayId());
    timeLayout->addWidget(new QLabel("H:"));
    timeLayout->addWidget(hoursSpin);

    QSpinBox* minutesSpin = new QSpinBox();
    minutesSpin->setRange(0, 59);
    minutesSpin->setValue(p->minutes);
    timeLayout->addWidget(new QLabel("M:"));
    timeLayout->addWidget(minutesSpin);
    connect(minutesSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [overlay](int v) {
                auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());
                if (p) p->minutes = v;
            });

    QSpinBox* secondsSpin = new QSpinBox();
    secondsSpin->setRange(0, 59);
    secondsSpin->setValue(p->seconds);
    timeLayout->addWidget(new QLabel("S:"));
    timeLayout->addWidget(secondsSpin);
    connect(secondsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [overlay](int v) {
                auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());
                if (p) p->seconds = v;
            });

    layout->addLayout(timeLayout);

    QPushButton* textColorBtn = new QPushButton("Pick Text Color");
    layout->addWidget(textColorBtn);
    connect(textColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->textColor, this, "Select Text Color");
                if (!c.isValid()) return;
                p->textColor = c;
            });

    QPushButton* bgColorBtn = new QPushButton("Pick BG Color");
    layout->addWidget(bgColorBtn);
    connect(bgColorBtn, &QPushButton::clicked, this,
            [this, overlay]() {
                auto* p = dynamic_cast<CountdownParameters*>(overlay->getParameters());
                if (!p) return;
                QColor c = QColorDialog::getColor(p->backgroundColor, this, "Select Background Color");
                if (!c.isValid()) return;
                p->backgroundColor = c;
                uint32_t argb =
                    (uint32_t(c.blue()) |
                     (uint32_t(c.green()) << 8) |
                     (uint32_t(c.red()) << 16) |
                     (uint32_t(c.alpha()) << 24));
                overlay->updateChildSourceProperty("Background", "color", QVariant::fromValue((int64_t)argb));
            });

    return widget;
}

QPushButton* OverseerDock::createColorPickerButton(const QString& text, const QColor& initialColor) {
    QPushButton* button = new QPushButton(text);
    button->setProperty("overlayId", generateOverlayId());

    // Set button color as background
    QString style = QString("QPushButton { background-color: %1; color: %2; }")
        .arg(initialColor.name())
        .arg(initialColor.lightness() > 128 ? "black" : "white");
    button->setStyleSheet(style);

    connect(button, &QPushButton::clicked, this, &OverseerDock::onColorChanged);
    return button;
}

QPushButton* OverseerDock::createFilePickerButton(const QString& text, const QString& initialFile) {
    QPushButton* button = new QPushButton(text);
    button->setProperty("overlayId", generateOverlayId());
    button->setProperty("currentFile", initialFile);

    if (!initialFile.isEmpty()) {
        button->setText(QString("%1: %2").arg(text, QFileInfo(initialFile).fileName()));
    }

    connect(button, &QPushButton::clicked, this, &OverseerDock::onImageFileChanged);
    return button;
}

// Event handlers implementation
void OverseerDock::onPositionXChanged(int value) {
    // Implementation for position X changes
}

void OverseerDock::onPositionYChanged(int value) {
    // Implementation for position Y changes
}

void OverseerDock::onScaleChanged(double value) {
    // Implementation for scale changes
}

void OverseerDock::onVisibilityChanged(bool visible) {
    // Implementation for visibility changes
}

void OverseerDock::onRemoveOverlayClicked() {
    // Implementation for removing overlays
}

void OverseerDock::onTextChanged() {
    // Implementation for text changes
}

void OverseerDock::onColorChanged() {
    // Implementation for color changes
}

void OverseerDock::onFontChanged() {
    // Implementation for font changes
}

void OverseerDock::onImageFileChanged() {
    // Implementation for image file changes
}

void OverseerDock::onCountdownChanged() {
    // Implementation for countdown changes
}

void OverseerDock::onAutoFillTextChanged(const QString& text) {
    // Implementation for auto-fill
}

// Utility methods
QString OverseerDock::getUniqueOverlayName(OverlayType type) {
    static QMap<OverlayType, int> counters;
    int count = counters[type]++;
    return QString("%1-%2").arg(overlayTypeToString(type).toLower()).arg(count);
}

int OverseerDock::generateOverlayId() {
    static int nextId = 1;
    return nextId++;
}

OverseerOverlay* OverseerDock::findOverlayById(int id) {
    for (auto overlay : overlays) {
        if (overlay->getParameters()->overlayId.toInt() == id) {
            return overlay;
        }
    }
    return nullptr;
}

void OverseerDock::loadDataFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Data File", "",
                                                   "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    if (dataIngestor->loadFile(fileName)) {
        QMessageBox::information(this, "Success", QString("Loaded %1 data rows").arg(dataIngestor->getRowCount()));
        updateAutoFillCompleters();
    } else {
        QMessageBox::critical(this, "Error", "Failed to load data file");
    }
}

void OverseerDock::updateAutoFillCompleters() {
    // Update completers for existing text fields
}

void OverseerDock::applyAutoFill(const IngestedDataRow& row) {
    // Apply auto-filled data to current overlay
}

void OverseerDock::saveSettings() {
    settings->beginGroup("Overlays");
    settings->remove(""); // Clear existing

    for (int i = 0; i < overlays.size(); ++i) {
        OverseerOverlay* overlay = overlays[i];
        OverlayParameters* params = overlay->getParameters();

        settings->beginGroup(QString("Overlay_%1").arg(i));
        settings->setValue("type", static_cast<int>(params->type));
        settings->setValue("name", params->overlayName);
        settings->setValue("data", params->toVariantMap());
        settings->endGroup();
    }
    settings->endGroup();
}

void OverseerDock::loadSettings() {
    settings->beginGroup("Overlays");
    QStringList groups = settings->childGroups();

    for (const QString& group : groups) {
        settings->beginGroup(group);
        OverlayType type = static_cast<OverlayType>(settings->value("type").toInt());
        QString name = settings->value("name").toString();
        QVariantMap data = settings->value("data").toMap();

        // Create overlay and restore parameters
        addOverlay(type);
        if (!overlays.isEmpty()) {
            OverseerOverlay* overlay = overlays.last();
            OverlayParameters* params = overlay->getParameters();
            params->fromVariantMap(data);
            // Update UI controls to match loaded parameters
        }

        settings->endGroup();
    }
    settings->endGroup();
}
