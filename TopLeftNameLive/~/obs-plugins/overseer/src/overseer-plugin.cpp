#include <obs-module.h>
#include <obs-frontend-api.h>
#include <util/base.h>
#include <QtWidgets/QMainWindow>
#include "overseer-dock.h"

OBS_DECLARE_MODULE()

static OverseerDock* g_dock = nullptr;

static void frontendEventCallback(enum obs_frontend_event event, void *private_data) {
    blog(LOG_INFO, "[OVERSEER] Frontend event received: %d", (int)event);

    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
        blog(LOG_INFO, "[OVERSEER] OBS finished loading, creating dock");

        if (!g_dock) {
            QMainWindow* mainWindow = static_cast<QMainWindow*>(obs_frontend_get_main_window());
            if (!mainWindow) {
                blog(LOG_ERROR, "[OVERSEER] No main window available");
                return;
            }

            blog(LOG_INFO, "[OVERSEER] Creating OverseerDock");
            g_dock = new OverseerDock(mainWindow);
            
            if (!g_dock) {
                blog(LOG_ERROR, "[OVERSEER] Failed to create OverseerDock");
                return;
            }

            blog(LOG_INFO, "[OVERSEER] Adding dock to OBS frontend");
            obs_frontend_add_dock(g_dock);
            
            blog(LOG_INFO, "[OVERSEER] Calling initializeWithOBS");
            g_dock->initializeWithOBS();
            
            blog(LOG_INFO, "[OVERSEER] Dock created and initialized successfully");
        } else {
            blog(LOG_WARNING, "[OVERSEER] Dock already exists, skipping creation");
        }
    } else if (event == OBS_FRONTEND_EVENT_EXIT) {
        blog(LOG_INFO, "[OVERSEER] OBS exit event received");
        if (g_dock) {
            g_dock->deleteLater();
            g_dock = nullptr;
        }
    }
}

bool obs_module_load(void) {
    blog(LOG_INFO, "[OVERSEER] Module load called");
    obs_frontend_add_event_callback(frontendEventCallback, nullptr);
    blog(LOG_INFO, "[OVERSEER] Frontend event callback registered");
    return true;
}

void obs_module_unload(void) {
    blog(LOG_INFO, "[OVERSEER] Module unload called");
    if (g_dock) {
        g_dock->deleteLater();
        g_dock = nullptr;
    }
}

const char* obs_module_name(void) {
    return "Overseer Plugin";
}

const char* obs_module_description(void) {
    return "Professional Overlay Management for OBS Studio";
}

#include "overseer-plugin.moc"