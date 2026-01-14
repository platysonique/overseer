#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QTimer>
#include <util/base.h>
#include <QSettings>
#include "overseer-dock.h"
// Forward declaration

class OverseerDock;

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

    ingestDataBtn_ = new QPushButton("Ingest Contestant Data");

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

    int id = overlays_.size() + 1;

    OverlayInstance overlay = {id, type, nullptr, QMap<QString, QVariant>(), false};

    overlays_.append(overlay);

    addOverlayControl(overlay);

    blog(LOG_INFO, "[OVERSEER] Added overlay %d of type %d", id, (int)type);

}



void OverseerDock::addOverlayControl(const OverlayInstance& overlay) {

    QGroupBox* group = new QGroupBox(QString("Overlay %1").arg(overlay.id));

    QVBoxLayout* layout = new QVBoxLayout(group);



    // Show/Hide buttons

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* showBtn = new QPushButton("SHOW");

    QPushButton* hideBtn = new QPushButton("HIDE");

    buttonLayout->addWidget(showBtn);

    buttonLayout->addWidget(hideBtn);

    layout->addLayout(buttonLayout);



    // Connect buttons

    connect(showBtn, &QPushButton::clicked, [this, overlay]() {

        toggleVisibility(overlay.id, true);

    });

    connect(hideBtn, &QPushButton::clicked, [this, overlay]() {

        toggleVisibility(overlay.id, false);

    });



    controlLayout_->addWidget(group);

}



void OverseerDock::toggleVisibility(int id, bool visible) {

    for (auto& overlay : overlays_) {

        if (overlay.id == id) {

            overlay.visible = visible;

            if (overlay.browserSource) {

                obs_source_set_enabled(overlay.browserSource, visible);

            }

            blog(LOG_INFO, "[OVERSEER] Overlay %d visibility set to %s", id, visible ? "visible" : "hidden");

            break;

        }

    }

}



void OverseerDock::loadSettings() {

    settings_ = new QSettings("Pombomb Media", "Overseer Plugin");

}



void OverseerDock::saveSettings() {

    if (settings_) {

        delete settings_;

        settings_ = nullptr;

    }

}



void OverseerDock::ingestData() {

    blog(LOG_INFO, "[OVERSEER] Ingesting contestant data");

    // TODO: Implement data ingestion

}



// Global dock pointer


#include <QtWidgets/QGroupBox>

#include <QtWidgets/QComboBox>

#include <QtWidgets/QScrollArea>

#include <QtWidgets/QHBoxLayout>

#include <QtWidgets/QLabel>

#include <QtWidgets/QLineEdit>

#include <QtWidgets/QSpinBox>

#include <QtWidgets/QCheckBox>
// Forward declaration







// Main OverseerDock class




// Global dock pointer


#include <QtWidgets/QGroupBox>

#include <QMap>

#include <QSettings>

#include <QDir>

#include <obs.h>

#include <obs-source.h>


#include <util/platform.h>

#include <util/dstr.h>

// Global dock pointer
static OverseerDock* dock = nullptr;

// Event callback - creates Qt widgets AFTER QApplication is ready
void on_obs_ready(enum obs_frontend_event event, void* data) {
    fprintf(stderr, "[OVERSEER] STDERR: Frontend event received: %d\n", (int)event);
    blog(LOG_INFO, "[OVERSEER] Frontend event received: %d", (int)event);
    
    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING || event == 25 || event == 26) {
        fprintf(stderr, "[OVERSEER] STDERR: FINISHED_LOADING event received, creating dock immediately\n");
        blog(LOG_INFO, "[OVERSEER] FINISHED_LOADING event received, creating dock immediately");

        // Get main window (now safe)
        auto* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
        if (!main_window) {
            fprintf(stderr, "[OVERSEER] STDERR: No main window available\n");
            blog(LOG_ERROR, "[OVERSEER] No main window available");
            return;
        }
        fprintf(stderr, "[OVERSEER] STDERR: Main window found, creating dock\n");

        // Create OverseerDock
        dock = new OverseerDock(main_window);
        if (!dock) {
            fprintf(stderr, "[OVERSEER] STDERR: Failed to create dock\n");
            return;
        }
        fprintf(stderr, "[OVERSEER] STDERR: Dock created, adding to frontend\n");
        obs_frontend_add_dock(dock);
        fprintf(stderr, "[OVERSEER] STDERR: Dock added to frontend\n");
        blog(LOG_INFO, "[OVERSEER] Overseer dock created and added");

        // Create a test overlay to verify the plugin is working
        fprintf(stderr, "[OVERSEER] STDERR: Creating test overlay\n");
        dock->addOverlay(LOGO);  // Create a LOGO overlay
        fprintf(stderr, "[OVERSEER] STDERR: Test overlay created\n");

        dock->setupUI();
        dock->loadSettings();
        fprintf(stderr, "[OVERSEER] STDERR: Dock setup complete\n");
    }
}

// CORRECT: Move Qt widget creation to event callback
bool obs_module_load(void) {
    printf("[OVERSEER] Module load called\n");
    fflush(stdout);
    fprintf(stderr, "[OVERSEER] STDERR: Module load called\n");
    blog(LOG_INFO, "[OVERSEER] Plugin loading started");

    // Register callback to wait for OBS UI to be ready
    obs_frontend_add_event_callback(on_obs_ready, nullptr);

    blog(LOG_INFO, "[OVERSEER] Frontend event callback registered");
    return true;
}

void obs_module_unload(void) {
    if (dock) {
        delete dock;
        dock = nullptr;
    }
}

MODULE_EXPORT uint32_t obs_module_ver(void) {
    return LIBOBS_API_VER;
}

MODULE_EXPORT void obs_module_set_pointer(obs_module_t *module) {

    // Required OBS module function - OBS will set this pointer
}


// Force vtable instantiation
void* force_vtable_instantiation() {
    return new OverseerDock(nullptr);
}

