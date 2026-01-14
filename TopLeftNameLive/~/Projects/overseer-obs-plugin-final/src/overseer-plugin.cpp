#include <obs-module.h>
#include <obs-frontend-api.h>
#include <obs.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFileDialog>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <util/base.h>
#include <QSettings>
#include "overseer-dock.h"

// Global dock pointer
static OverseerDock* dock = nullptr;

// Event callback - creates Qt widgets AFTER QApplication is ready
void on_obs_ready(enum obs_frontend_event event, void* data) {
    blog(LOG_INFO, "[OVERSEER] Frontend event received: %d", (int)event);

    switch (event) {
        case OBS_FRONTEND_EVENT_FINISHED_LOADING:
            blog(LOG_INFO, "[OVERSEER] FINISHED_LOADING event received, scheduling dock creation");

            // Use QTimer to delay Qt widget creation even further (like StreamFX)
            QTimer::singleShot(1000, []() {
                blog(LOG_INFO, "[OVERSEER] Timer fired, creating Qt dock now");

                // Get main window (now safe)
                auto* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
                if (!main_window) {
                    blog(LOG_ERROR, "[OVERSEER] No main window available");
                    return;
                }

                // Create OverseerDock
                dock = new OverseerDock(main_window);
                obs_frontend_add_dock(dock);
                blog(LOG_INFO, "[OVERSEER] Overseer dock created and added");
                dock->setupUI();
                dock->loadSettings();
            });
            break;

        case OBS_FRONTEND_EVENT_EXIT:
            blog(LOG_INFO, "[OVERSEER] OBS exit event received, cleaning up");
            // Let OBS handle dock cleanup - don't manually delete
            dock = nullptr;
            break;
    }
}

// CORRECT: Move Qt widget creation to event callback
bool obs_module_load(void) {
    printf("[OVERSEER] Module load called\n");
    fflush(stdout);
    blog(LOG_INFO, "[OVERSEER] Plugin loading started");

    // Register callback to wait for OBS UI to be ready
    obs_frontend_add_event_callback(on_obs_ready, nullptr);

    blog(LOG_INFO, "[OVERSEER] Frontend event callback registered");
    return true;
}

void obs_module_unload(void) {
    blog(LOG_INFO, "[OVERSEER] Plugin unloading");

    // Remove event callback
    obs_frontend_remove_event_callback(on_obs_ready, nullptr);

    // Note: Don't delete dock here - OBS handles Qt cleanup
    blog(LOG_INFO, "[OVERSEER] Plugin unloaded successfully");
}

MODULE_EXPORT uint32_t obs_module_ver(void) {
    return LIBOBS_API_VER;
}

MODULE_EXPORT void obs_module_set_pointer(obs_module_t *module) {
    // Required OBS module function - OBS will set this pointer
}

// OverseerDock implementation
OverseerDock::OverseerDock(QWidget* parent) : QDockWidget("Overseer Control", parent) {
    blog(LOG_INFO, "[OVERSEER] OverseerDock constructor start");

    setupUI();
    loadSettings();

    blog(LOG_INFO, "[OVERSEER] OverseerDock constructor completed");
}

OverseerDock::~OverseerDock() {
    saveSettings();
}

void OverseerDock::setupUI() {
    QWidget* widget = new QWidget();
    mainLayout_ = new QVBoxLayout(widget);

    // Overlay type selection
    QHBoxLayout* typeLayout = new QHBoxLayout();
    overlayTypeSelect_ = new QComboBox();

    overlayTypeSelect_->addItem("Logo", LOGO);
    overlayTypeSelect_->addItem("Lower Third", LOWER_THIRD);
    overlayTypeSelect_->addItem("Live Bug", LIVE_BUG);
    overlayTypeSelect_->addItem("Big Lower 3rd", BIG_LOWER_3RD);
    overlayTypeSelect_->addItem("Countdown", COUNTDOWN);

    addOverlayBtn_ = new QPushButton("Add Overlay");
    connect(addOverlayBtn_, &QPushButton::clicked, this, &OverseerDock::onAddOverlayClicked);

    typeLayout->addWidget(new QLabel("Type:"));
    typeLayout->addWidget(overlayTypeSelect_);
    typeLayout->addWidget(addOverlayBtn_);
    mainLayout_->addLayout(typeLayout);

    // Ingest data button
    ingestDataBtn_ = new QPushButton("Ingest Data");
    connect(ingestDataBtn_, &QPushButton::clicked, this, &OverseerDock::onIngestDataClicked);
    mainLayout_->addWidget(ingestDataBtn_);

    // Scroll area for overlay controls
    scrollArea_ = new QScrollArea();
    scrollContent_ = new QWidget();
    controlLayout_ = new QVBoxLayout(scrollContent_);
    scrollArea_->setWidget(scrollContent_);
    scrollArea_->setWidgetResizable(true);
    mainLayout_->addWidget(scrollArea_);

    setWidget(widget);
}

void OverseerDock::onAddOverlayClicked() {
    OverlayType type = (OverlayType)overlayTypeSelect_->currentData().toInt();
    addOverlay(type);
}

void OverseerDock::onIngestDataClicked() {
    ingestData();
}

void OverseerDock::addOverlay(OverlayType type) {
    OverlayInstance instance;
    instance.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    instance.type = type;
    instance.position = BOTTOM_CENTER;

    // Create OBS source based on overlay type
    createOverlaySource(instance);

    overlays_.append(instance);

    // Create control widget
    QWidget* controlWidget = createOverlayControl(instance);
    controlLayout_->addWidget(controlWidget);

    saveSettings();
    blog(LOG_INFO, "[OVERSEER] Overlay added: %s", instance.id.toUtf8().constData());
}

QWidget* OverseerDock::createOverlayControl(const OverlayInstance& instance) {
    QGroupBox* group = new QGroupBox(getOverlayTypeName(instance.type));
    QVBoxLayout* layout = new QVBoxLayout(group);

    // Basic controls
    QHBoxLayout* basicLayout = new QHBoxLayout();

    QPushButton* showBtn = new QPushButton("SHOW");
    QPushButton* hideBtn = new QPushButton("HIDE");
    QPushButton* removeBtn = new QPushButton("×");

    connect(showBtn, &QPushButton::clicked, [this, instance_id = instance.id]() { toggleVisibility(instance_id, true); });
    connect(hideBtn, &QPushButton::clicked, [this, instance_id = instance.id]() { toggleVisibility(instance_id, false); });
    connect(removeBtn, &QPushButton::clicked, [this, instance_id = instance.id]() { removeOverlay(instance_id); });

    basicLayout->addWidget(showBtn);
    basicLayout->addWidget(hideBtn);
    basicLayout->addWidget(removeBtn);
    basicLayout->addStretch();

    layout->addLayout(basicLayout);

    // Parameter controls
    createParameterControls(layout, instance);

    return group;
}

void OverseerDock::createParameterControls(QVBoxLayout* layout, const OverlayInstance& instance) {
    if (instance.type == LOGO) {
        // Media file selection for logos
        QHBoxLayout* mediaLayout = new QHBoxLayout();
        mediaLayout->addWidget(new QLabel("Image File:"));
        QString currentFile = instance.params.value("imageFile", "/home/papaya/Projects/circle.png").toString();
        QString buttonText = currentFile.isEmpty() ? "Choose Media..." : QFileInfo(currentFile).fileName();
        QPushButton* selectFileBtn = new QPushButton(buttonText);
        mediaLayout->addWidget(selectFileBtn);
        layout->addLayout(mediaLayout);

        connect(selectFileBtn, &QPushButton::clicked, [this, instance_id = instance.id, selectFileBtn]() {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Media File", "",
                "All Supported Files (*.png *.jpg *.jpeg *.bmp *.gif *.tiff *.mp4 *.avi *.mov *.mkv *.webm);;Image Files (*.png *.jpg *.jpeg *.bmp *.tiff);;Animated Images (*.gif);;Video Files (*.mp4 *.avi *.mov *.mkv *.webm);;All Files (*)");
            if (!fileName.isEmpty()) {
                int idx = getOverlayIndex(instance_id);
                if (idx >= 0) {
                    overlays_[idx].params["imageFile"] = fileName;
                    selectFileBtn->setText(QFileInfo(fileName).fileName());
                    saveSettings();

                    // Update existing source with new file
                    if (overlays_[idx].source) {
                        obs_data_t* settings = obs_data_create();
                        obs_data_set_string(settings, "file", fileName.toUtf8().constData());
                        obs_source_update(overlays_[idx].source, settings);
                        obs_data_release(settings);
                        blog(LOG_INFO, "[OVERSEER] Updated overlay %s with new media file: %s",
                             instance_id.toUtf8().constData(), fileName.toUtf8().constData());
                    }
                }
            }
        });
    } else {
        // Text inputs for text-based overlays
        QHBoxLayout* textLayout = new QHBoxLayout();
        textLayout->addWidget(new QLabel("Top Text:"));
        QLineEdit* topText = new QLineEdit(instance.params.value("topText", "").toString());
        textLayout->addWidget(topText);
        layout->addLayout(textLayout);

        QHBoxLayout* bottomLayout = new QHBoxLayout();
        bottomLayout->addWidget(new QLabel("Bottom Text:"));
        QLineEdit* bottomText = new QLineEdit(instance.params.value("bottomText", "").toString());
        bottomLayout->addWidget(bottomText);
        layout->addLayout(bottomLayout);

        // Connect text changes
        connect(topText, &QLineEdit::textChanged, [this, instance_id = instance.id](const QString& text) {
            int idx = getOverlayIndex(instance_id);
            if (idx >= 0) {
                overlays_[idx].params["topText"] = text;
                saveSettings();
            }
        });
        connect(bottomText, &QLineEdit::textChanged, [this, instance_id = instance.id](const QString& text) {
            int idx = getOverlayIndex(instance_id);
            if (idx >= 0) {
                overlays_[idx].params["bottomText"] = text;
                saveSettings();
            }
        });
    }

    // Professional styling controls
    QGroupBox* styleGroup = new QGroupBox("Professional Styling");
    QVBoxLayout* styleLayout = new QVBoxLayout(styleGroup);

    // Background style selector
    QHBoxLayout* bgLayout = new QHBoxLayout();
    bgLayout->addWidget(new QLabel("Background:"));
    QComboBox* bgStyle = new QComboBox();
    bgStyle->addItem("Solid Color", "solid");
    bgStyle->addItem("Gradient", "gradient");
    bgStyle->addItem("Glass Effect", "glass");
    bgStyle->addItem("Professional", "pro");
    bgStyle->setCurrentText(instance.params.value("bgStyle", "pro").toString());
    bgLayout->addWidget(bgStyle);
    styleLayout->addLayout(bgLayout);

    // Border/Shadow controls
    QHBoxLayout* effectLayout = new QHBoxLayout();
    QCheckBox* borderEnabled = new QCheckBox("Border");
    borderEnabled->setChecked(instance.params.value("borderEnabled", true).toBool());
    QCheckBox* shadowEnabled = new QCheckBox("Shadow");
    shadowEnabled->setChecked(instance.params.value("shadowEnabled", true).toBool());
    effectLayout->addWidget(borderEnabled);
    effectLayout->addWidget(shadowEnabled);
    styleLayout->addLayout(effectLayout);

    // Animation controls
    QHBoxLayout* animLayout = new QHBoxLayout();
    animLayout->addWidget(new QLabel("Animation:"));
    QComboBox* animation = new QComboBox();
    animation->addItem("None", "none");
    animation->addItem("Fade In", "fade");
    animation->addItem("Slide In", "slide");
    animation->addItem("Scale In", "scale");
    animation->setCurrentText(instance.params.value("animation", "fade").toString());
    animLayout->addWidget(animation);
    styleLayout->addLayout(animLayout);

    layout->addWidget(styleGroup);

    // Connect styling controls
    connect(bgStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, instance_id = instance.id, bgStyle](int index) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["bgStyle"] = bgStyle->currentData().toString();
            saveSettings();
        }
    });
    connect(borderEnabled, &QCheckBox::toggled, [this, instance_id = instance.id](bool checked) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["borderEnabled"] = checked;
            saveSettings();
        }
    });
    connect(shadowEnabled, &QCheckBox::toggled, [this, instance_id = instance.id](bool checked) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["shadowEnabled"] = checked;
            saveSettings();
        }
    });
    connect(animation, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, instance, animation](int index) {
        overlays_[getOverlayIndex(instance.id)].params["animation"] = animation->currentData().toString();
        saveSettings();
    });

    // Typography controls for production text
    QGroupBox* typographyGroup = new QGroupBox("Professional Typography");
    QVBoxLayout* typographyLayout = new QVBoxLayout(typographyGroup);

    // Font family
    QHBoxLayout* fontLayout = new QHBoxLayout();
    fontLayout->addWidget(new QLabel("Font:"));
    QComboBox* fontFamily = new QComboBox();
    fontFamily->addItem("Arial", "Arial");
    fontFamily->addItem("Helvetica", "Helvetica");
    fontFamily->addItem("Verdana", "Verdana");
    fontFamily->addItem("Georgia", "Georgia");
    fontFamily->addItem("Times New Roman", "Times New Roman");
    fontFamily->addItem("Impact", "Impact");
    fontFamily->setCurrentText(instance.params.value("fontFamily", "Arial").toString());
    fontLayout->addWidget(fontFamily);
    typographyLayout->addLayout(fontLayout);

    // Font size and weight
    QHBoxLayout* sizeWeightLayout = new QHBoxLayout();
    sizeWeightLayout->addWidget(new QLabel("Size:"));
    QSpinBox* fontSize = new QSpinBox();
    fontSize->setRange(12, 120);
    fontSize->setValue(instance.params.value("fontSize", 36).toInt());
    sizeWeightLayout->addWidget(fontSize);

    sizeWeightLayout->addWidget(new QLabel("Weight:"));
    QComboBox* fontWeight = new QComboBox();
    fontWeight->addItem("Normal", QFont::Normal);
    fontWeight->addItem("Bold", QFont::Bold);
    fontWeight->addItem("Light", QFont::Light);
    fontWeight->setCurrentIndex(fontWeight->findData(instance.params.value("fontWeight", QFont::Bold).toInt()));
    sizeWeightLayout->addWidget(fontWeight);
    typographyLayout->addLayout(sizeWeightLayout);

    // Text colors
    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Text Color:"));
    QPushButton* textColorBtn = new QPushButton("Text");
    QColor textColor = instance.params.value("textColor", QColor(255, 255, 255)).value<QColor>();
    QString textStyle = QString("background-color: rgb(%1, %2, %3); color: black;")
                       .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
    textColorBtn->setStyleSheet(textStyle);
    colorLayout->addWidget(textColorBtn);

    colorLayout->addWidget(new QLabel("Outline:"));
    QPushButton* outlineColorBtn = new QPushButton("Outline");
    QColor outlineColor = instance.params.value("outlineColor", QColor(0, 0, 0)).value<QColor>();
    QString outlineStyle = QString("background-color: rgb(%1, %2, %3); color: white;")
                          .arg(outlineColor.red()).arg(outlineColor.green()).arg(outlineColor.blue());
    outlineColorBtn->setStyleSheet(outlineStyle);
    colorLayout->addWidget(outlineColorBtn);
    typographyLayout->addLayout(colorLayout);

    layout->addWidget(typographyGroup);

    // Connect typography controls
    connect(fontFamily, &QComboBox::currentTextChanged, [this, instance_id = instance.id](const QString& text) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["fontFamily"] = text;
            saveSettings();
        }
    });
    connect(fontSize, QOverload<int>::of(&QSpinBox::valueChanged), [this, instance_id = instance.id](int value) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["fontSize"] = value;
            saveSettings();
        }
    });
    connect(fontWeight, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, instance_id = instance.id, fontWeight](int index) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["fontWeight"] = fontWeight->currentData().toInt();
            saveSettings();
        }
    });
    connect(textColorBtn, &QPushButton::clicked, [this, instance_id = instance.id, textColorBtn]() {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            QColor color = QColorDialog::getColor(overlays_[idx].params.value("textColor", QColor(255, 255, 255)).value<QColor>(),
                                                 this, "Choose Text Color");
            if (color.isValid()) {
                overlays_[idx].params["textColor"] = color;
                QString style = QString("background-color: rgb(%1, %2, %3); color: black;")
                               .arg(color.red()).arg(color.green()).arg(color.blue());
                textColorBtn->setStyleSheet(style);
                saveSettings();
            }
        }
    });
    connect(outlineColorBtn, &QPushButton::clicked, [this, instance_id = instance.id, outlineColorBtn]() {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            QColor color = QColorDialog::getColor(overlays_[idx].params.value("outlineColor", QColor(0, 0, 0)).value<QColor>(),
                                                 this, "Choose Outline Color");
            if (color.isValid()) {
                overlays_[idx].params["outlineColor"] = color;
                QString style = QString("background-color: rgb(%1, %2, %3); color: white;")
                               .arg(color.red()).arg(color.green()).arg(color.blue());
                outlineColorBtn->setStyleSheet(style);
                saveSettings();
            }
        }
    });

    // Size controls
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel("Width:"));
    QSpinBox* widthSpin = new QSpinBox();
    widthSpin->setRange(50, 2000);
    widthSpin->setValue(instance.params.value("width", 200).toInt());
    sizeLayout->addWidget(widthSpin);

    sizeLayout->addWidget(new QLabel("Height:"));
    QSpinBox* heightSpin = new QSpinBox();
    heightSpin->setRange(50, 2000);
    heightSpin->setValue(instance.params.value("height", 100).toInt());
    sizeLayout->addWidget(heightSpin);

    layout->addLayout(sizeLayout);

    // Connect size controls
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this, instance_id = instance.id](int value) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["width"] = value;
            saveSettings();
        }
    });
    connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this, instance_id = instance.id](int value) {
        int idx = getOverlayIndex(instance_id);
        if (idx >= 0) {
            overlays_[idx].params["height"] = value;
            saveSettings();
        }
    });
}

void OverseerDock::toggleVisibility(const QString& id, bool visible) {
    for (int i = 0; i < overlays_.size(); ++i) {
        if (overlays_[i].id == id) {
            overlays_[i].visible = visible;
            // Update OBS source visibility
            if (overlays_[i].source) {
                obs_source_set_enabled(overlays_[i].source, visible);
            }
            saveSettings();
            blog(LOG_INFO, "[OVERSEER] Toggled visibility for %s: %s", id.toUtf8().constData(), visible ? "SHOW" : "HIDE");
            break;
        }
    }
}

void OverseerDock::createOverlaySource(OverlayInstance& instance) {
    obs_data_t* settings = obs_data_create();

    const char* source_id = nullptr;

    switch (instance.type) {
        case LOGO: {
            source_id = "image_source";
            // Use the default circle.png from projects directory
            QString defaultImage = "/home/papaya/Projects/circle.png";
            QString currentImage = instance.params.value("imageFile", defaultImage).toString();
            obs_data_set_string(settings, "file", currentImage.toUtf8().constData());
            break;
        }
        case LOWER_THIRD:
        case BIG_LOWER_3RD:
        case LIVE_BUG:
        case COUNTDOWN:
            source_id = "text_ft2_source_v2";
            obs_data_set_string(settings, "text", "TEST TEXT");
            obs_data_set_int(settings, "font_size", 48);
            obs_data_set_string(settings, "font_face", "Arial");
            break;
    }

    if (source_id) {
        QString source_name = QString("Overseer_%1").arg(instance.id);
        instance.source = obs_source_create(source_id, source_name.toUtf8().constData(), settings, nullptr);

        if (instance.source) {
            blog(LOG_INFO, "[OVERSEER] Created OBS source for overlay: %s", instance.id.toUtf8().constData());

            // Add to current scene
            obs_source_t* scene_source = obs_frontend_get_current_scene();
            if (scene_source) {
                obs_scene_t* scene = obs_scene_from_source(scene_source);
                if (scene) {
                    obs_scene_add(scene, instance.source);
                    blog(LOG_INFO, "[OVERSEER] Added source to scene: %s", instance.id.toUtf8().constData());
                }
                obs_source_release(scene_source);
            }

            // Don't set initial visibility here - let toggleVisibility handle it
        } else {
            blog(LOG_ERROR, "[OVERSEER] Failed to create OBS source for overlay: %s", instance.id.toUtf8().constData());
        }
    }

    obs_data_release(settings);
}

void OverseerDock::removeOverlay(const QString& id) {
    for (int i = 0; i < overlays_.size(); ++i) {
        if (overlays_[i].id == id) {
            // Disable source (don't remove from scene to avoid crashes)
            if (overlays_[i].source) {
                obs_source_set_enabled(overlays_[i].source, false);
                // Note: We don't remove from scene to avoid potential crashes
                // OBS will clean up sources on shutdown
            }

            overlays_.removeAt(i);
            // Remove widget from layout
            QWidget* widget = controlLayout_->itemAt(i)->widget();
            controlLayout_->removeWidget(widget);
            widget->deleteLater();
            saveSettings();
            blog(LOG_INFO, "[OVERSEER] Removed overlay: %s", id.toUtf8().constData());
            break;
        }
    }
}

void OverseerDock::loadSettings() {
    settings_ = new QSettings("Pombomb Media", "Overseer Plugin");

    QJsonArray overlaysArray = settings_->value("overlays").toJsonArray();
    for (const QJsonValue& value : overlaysArray) {
        QJsonObject obj = value.toObject();
        OverlayInstance instance;
        instance.id = obj["id"].toString();
        instance.type = (OverlayType)obj["type"].toInt();
        instance.position = (Position)obj["position"].toInt();
        instance.visible = obj["visible"].toBool();
        instance.params = obj["params"].toObject().toVariantMap();

        overlays_.append(instance);
        QWidget* controlWidget = createOverlayControl(instance);
        controlLayout_->addWidget(controlWidget);
    }
}

void OverseerDock::saveSettings() {
    if (!settings_) return;

    QJsonArray overlaysArray;
    for (const auto& overlay : overlays_) {
        QJsonObject obj;
        obj["id"] = overlay.id;
        obj["type"] = overlay.type;
        obj["position"] = overlay.position;
        obj["visible"] = overlay.visible;
        obj["params"] = QJsonObject::fromVariantMap(overlay.params);
        overlaysArray.append(obj);
    }

    settings_->setValue("overlays", overlaysArray);
    settings_->sync();
}

int OverseerDock::getOverlayIndex(const QString& id) {
    for (int i = 0; i < overlays_.size(); ++i) {
        if (overlays_[i].id == id) {
            return i;
        }
    }
    return -1;
}

QString OverseerDock::getOverlayTypeName(OverlayType type) {
    switch (type) {
        case LOGO: return "Logo";
        case LOWER_THIRD: return "Lower Third";
        case LIVE_BUG: return "Live Bug";
        case BIG_LOWER_3RD: return "Big Lower 3rd";
        case COUNTDOWN: return "Countdown";
        default: return "Unknown";
    }
}

void OverseerDock::ingestData() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Data File", "",
        "Data Files (*.txt *.csv);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // TODO: Parse and store data for auto-fill
    blog(LOG_INFO, "[OVERSEER] Data ingested from: %s", fileName.toUtf8().constData());
}

