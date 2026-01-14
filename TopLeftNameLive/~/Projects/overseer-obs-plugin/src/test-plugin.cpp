#include <obs-module.h>
#include <obs-frontend-api.h>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <util/base.h>

// Global dock pointer
static QDockWidget* test_dock = nullptr;

// Event callback - creates Qt widgets AFTER QApplication is ready
void on_obs_ready(enum obs_frontend_event event, void* data) {
    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
        blog(LOG_INFO, "[TEST] OBS frontend finished loading, creating Qt dock now");

        // Get main window (now safe)
        auto* main_window = static_cast<QMainWindow*>(obs_frontend_get_main_window());
        if (!main_window) {
            blog(LOG_ERROR, "[TEST] No main window available");
            return;
        }

        // Create simple Qt dock (safe timing)
        test_dock = new QDockWidget("Test Dock", main_window);
        QPushButton* btn = new QPushButton("Hello OBS!");
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(btn);
        QWidget* widget = new QWidget();
        widget->setLayout(layout);
        test_dock->setWidget(widget);

        obs_frontend_add_dock(test_dock);
        blog(LOG_INFO, "[TEST] Test dock successfully created and added");
    }
}

// CORRECT: Move Qt widget creation to event callback
bool obs_module_load(void) {
    blog(LOG_INFO, "[TEST] Plugin loading started");

    // Register callback to wait for OBS UI to be ready
    obs_frontend_add_event_callback(on_obs_ready, nullptr);

    blog(LOG_INFO, "[TEST] Frontend event callback registered");
    return true;
}

void obs_module_unload(void) {
    if (test_dock) {
        delete test_dock;
        test_dock = nullptr;
    }
}



