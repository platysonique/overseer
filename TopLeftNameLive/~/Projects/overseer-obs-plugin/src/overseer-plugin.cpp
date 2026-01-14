#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpinBox>
#include <QtCore/QTimer>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <util/base.h>

// OBS Plugin Metadata - Required for OBS to load the plugin
// Custom locale implementation without obs_current_module
lookup_t *obs_module_lookup = NULL;
const char *obs_module_text(const char *val)
{
    const char *out = val;
    if (obs_module_lookup) {
        text_lookup_getstr(obs_module_lookup, val, &out);
    }
    return out;
}
bool obs_module_get_string(const char *val, const char **out)
{
    if (obs_module_lookup) {
        return text_lookup_getstr(obs_module_lookup, val, out);
    }
    *out = val;
    return true;
}
void obs_module_set_locale(const char *locale)
{
    if (obs_module_lookup) {
        text_lookup_destroy(obs_module_lookup);
    }
    // Skip locale loading for now - just initialize empty lookup
    obs_module_lookup = NULL;
}
void obs_module_free_locale(void)
{
    if (obs_module_lookup) {
        text_lookup_destroy(obs_module_lookup);
        obs_module_lookup = NULL;
    }
}

extern "C" {

const char *obs_module_name(void)
{
    return "overseer-plugin";
}

const char *obs_module_description(void)
{
    return "Modular Overlay Control for OBS";
}

const char *obs_module_author(void)
{
    return "Pombomb Media";
}

uint32_t obs_module_ver(void)
{
    // Return the API version that working plugins use (32.0.4)
    // instead of LIBOBS_API_VER (31.1.0) from headers
    return 0x20000004;
}

}

// Overlay types
enum OverlayType {
    LOGO,
    LOWER_THIRD,
    LIVE_BUG,
    BIG_LOWER_3RD,
    COUNTDOWN
};

// Position types
enum Position {
    UP_LEFT,
    UP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_CENTER
};

// Overlay instance structure
struct OverlayInstance {
    QString id;
    OverlayType type;
    Position position;
    bool visible;
    QVariantMap params;
    OverlayInstance() : visible(false) {}
};

// Global dock pointer
static QDockWidget* dock = nullptr;

// Forward declarations
class OverseerDock;

void on_obs_ready(enum obs_frontend_event event, void* data);

class OverseerDock : public QDockWidget {
    Q_OBJECT

public:
    OverseerDock(QWidget* parent = nullptr) : QDockWidget("Overseer Control", parent) {
        blog(LOG_INFO, "[OVERSEER] OverseerDock constructor start");

        setupUI();
        blog(LOG_INFO, "[OVERSEER] OverseerDock constructor completed");
    }

    ~OverseerDock() {
        saveSettings();
    }

private slots:
    void onAddOverlayClicked() {
        OverlayType type = (OverlayType)overlayTypeSelect_->currentData().toInt();
        addOverlay(type);
    }

    void onIngestDataClicked() {
        ingestData();
    }

private:
    QList<OverlayInstance> overlays_;
    QVBoxLayout* mainLayout_;
    QComboBox* overlayTypeSelect_;
    QPushButton* addOverlayBtn_;
    QPushButton* ingestDataBtn_;
    QScrollArea* scrollArea_;
    QWidget* scrollContent_;
    QVBoxLayout* controlLayout_;
    QSettings* settings_;

    void setupUI() {
        // Initialize settings NOW (Qt is available)
        settings_ = new QSettings("Pombomb Media", "Overseer Plugin");

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
        typeLayout->addStretch();

        mainLayout_->addLayout(typeLayout);

        // Ingest data button
        ingestDataBtn_ = new QPushButton("Ingest Data");
        connect(ingestDataBtn_, &QPushButton::clicked, this, &OverseerDock::onIngestDataClicked);
        mainLayout_->addWidget(ingestDataBtn_);

        // Scrollable controls area
        scrollArea_ = new QScrollArea();
        scrollContent_ = new QWidget();
        controlLayout_ = new QVBoxLayout(scrollContent_);
        scrollArea_->setWidget(scrollContent_);
        scrollArea_->setWidgetResizable(true);
        mainLayout_->addWidget(scrollArea_);

        setWidget(widget);
        blog(LOG_INFO, "[OVERSEER] UI setup completed");
    }

    void addOverlay(OverlayType type) {
        OverlayInstance instance;
        instance.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        instance.type = type;
        instance.position = BOTTOM_CENTER;

        overlays_.append(instance);

        // Create control widget
        QWidget* controlWidget = createOverlayControl(instance);
        controlLayout_->addWidget(controlWidget);

        saveSettings();
        blog(LOG_INFO, "[OVERSEER] Overlay added: %s", instance.id.toUtf8().constData());
    }

    QWidget* createOverlayControl(const OverlayInstance& instance) {
        QGroupBox* group = new QGroupBox(getOverlayTypeName(instance.type));
        QVBoxLayout* layout = new QVBoxLayout(group);

        // Basic controls
        QHBoxLayout* basicLayout = new QHBoxLayout();

        QPushButton* showBtn = new QPushButton("SHOW");
        QPushButton* hideBtn = new QPushButton("HIDE");
        QPushButton* removeBtn = new QPushButton("×");

        connect(showBtn, &QPushButton::clicked, [this, instance]() { toggleVisibility(instance.id, true); });
        connect(hideBtn, &QPushButton::clicked, [this, instance]() { toggleVisibility(instance.id, false); });
        connect(removeBtn, &QPushButton::clicked, [this, instance]() { removeOverlay(instance.id); });

        basicLayout->addWidget(showBtn);
        basicLayout->addWidget(hideBtn);
        basicLayout->addWidget(removeBtn);
        basicLayout->addStretch();

        layout->addLayout(basicLayout);

        // Parameter controls
        createParameterControls(layout, instance);

        return group;
    }

    void createParameterControls(QVBoxLayout* layout, const OverlayInstance& instance) {
        // Text inputs
        QHBoxLayout* textLayout = new QHBoxLayout();
        textLayout->addWidget(new QLabel("Top Text:"));
        QLineEdit* topText = new QLineEdit();
        textLayout->addWidget(topText);
        layout->addLayout(textLayout);

        QHBoxLayout* bottomLayout = new QHBoxLayout();
        bottomLayout->addWidget(new QLabel("Bottom Text:"));
        QLineEdit* bottomText = new QLineEdit();
        bottomLayout->addWidget(bottomText);
        layout->addLayout(bottomLayout);

        // Media controls
        QCheckBox* mediaEnabled = new QCheckBox("Enable Media");
        layout->addWidget(mediaEnabled);

        QHBoxLayout* mediaLayout = new QHBoxLayout();
        mediaLayout->addWidget(new QLabel("Media File:"));
        QLineEdit* mediaFile = new QLineEdit();
        QPushButton* browseBtn = new QPushButton("Browse");
        connect(browseBtn, &QPushButton::clicked, [mediaFile]() {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Media File", "",
                "Media Files (*.png *.jpg *.jpeg *.gif *.mp4 *.avi *.mov *.webm);;All Files (*)");
            if (!fileName.isEmpty()) {
                mediaFile->setText(fileName);
            }
        });
        mediaLayout->addWidget(mediaFile);
        mediaLayout->addWidget(browseBtn);
        layout->addLayout(mediaLayout);

        // Size controls
        QHBoxLayout* sizeLayout = new QHBoxLayout();
        sizeLayout->addWidget(new QLabel("Width:"));
        QSpinBox* widthSpin = new QSpinBox();
        widthSpin->setRange(50, 2000);
        widthSpin->setValue(200);
        sizeLayout->addWidget(widthSpin);

        sizeLayout->addWidget(new QLabel("Height:"));
        QSpinBox* heightSpin = new QSpinBox();
        heightSpin->setRange(50, 2000);
        heightSpin->setValue(100);
        sizeLayout->addWidget(heightSpin);

        layout->addLayout(sizeLayout);
    }

    void toggleVisibility(const QString& id, bool visible) {
        for (int i = 0; i < overlays_.size(); ++i) {
            if (overlays_[i].id == id) {
                overlays_[i].visible = visible;
                // TODO: Update OBS source visibility
                saveSettings();
                blog(LOG_INFO, "[OVERSEER] Toggled visibility for %s: %s", id.toUtf8().constData(), visible ? "SHOW" : "HIDE");
                break;
            }
        }
    }

    void removeOverlay(const QString& id) {
        for (int i = 0; i < overlays_.size(); ++i) {
            if (overlays_[i].id == id) {
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

    void ingestData() {
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

    QString getOverlayTypeName(OverlayType type) {
        switch (type) {
            case LOGO: return "Logo";
            case LOWER_THIRD: return "Lower Third";
            case LIVE_BUG: return "Live Bug";
            case BIG_LOWER_3RD: return "Big Lower 3rd";
            case COUNTDOWN: return "Countdown";
            default: return "Unknown";
        }
    }

    void saveSettings() {
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

    void loadSettings() {
        if (!settings_) return;

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
};

// MINIMAL TEST: Just log during load, no Qt objects
bool obs_module_load(void) {
    blog(LOG_INFO, "[OVERSEER] MINIMAL TEST: Plugin loading started - no Qt objects created");
    blog(LOG_INFO, "[OVERSEER] MINIMAL TEST: Module load successful");
    return true;
}

// Event callback - creates Qt widgets AFTER QApplication is ready
void on_obs_ready(enum obs_frontend_event event, void* data) {
    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
        blog(LOG_INFO, "[OVERSEER] OBS frontend finished loading, creating Qt dock now");

        // Get main window (now safe - QApplication exists)
        auto* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
        if (!main_window) {
            blog(LOG_ERROR, "[OVERSEER] No main window available even after frontend loaded");
            return;
        }

        // Create Qt dock NOW (safe timing - fixes the crash)
        dock = new OverseerDock(main_window);
        if (!dock) {
            blog(LOG_ERROR, "[OVERSEER] Failed to create OverseerDock");
            return;
        }

        obs_frontend_add_dock(dock);
        blog(LOG_INFO, "[OVERSEER] Overseer dock successfully created and added");
    }
}

void obs_module_unload(void) {
    blog(LOG_INFO, "[OVERSEER] MINIMAL TEST: Module unload");
}

#include "overseer-plugin.moc"