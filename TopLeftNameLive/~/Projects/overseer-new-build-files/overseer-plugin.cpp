#include <obs-module.h>

#include <obs-frontend-api.h>

#include "overseer-dock.h"



OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("overseer", "en-US")



static OverseerDock* g_dock = nullptr;



static void frontendEventCallback(enum obs_frontend_event event, void *private_data) {

    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {

        if (!g_dock) {

            g_dock = new OverseerDock();

            obs_frontend_add_dock_by_id("overseer_dock", "Overseer", g_dock);

            g_dock->initializeWithOBS();

        }

    }

}



bool obs_module_load(void) {

    obs_frontend_add_event_callback(frontendEventCallback, nullptr);

    return true;

}



void obs_module_unload(void) {

    if (g_dock) {

        g_dock->deleteLater();

        g_dock = nullptr;

    }

}



extern "C" void obs_module_set_locale(const char *locale) {}
