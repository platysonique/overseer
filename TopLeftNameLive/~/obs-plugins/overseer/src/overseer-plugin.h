#ifndef OVERSEER_PLUGIN_H
#define OVERSEER_PLUGIN_H

#include <obs.h>
#include <obs-frontend-api.h>
#include <QtCore/QTimer>

#include "overseer-dock.h"

// Forward declarations
class OverseerDock;

// Global plugin state
extern OverseerDock* overseerDock;

// OBS module declarations
OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("overseer-plugin", "en-US")

// Module functions
bool obs_module_load(void);
void obs_module_unload(void);

// Frontend event callback
void on_obs_frontend_event(enum obs_frontend_event event, void *private_data);

// OBS source registration (if needed)
// void register_overseer_sources(void);

#endif // OVERSEER_PLUGIN_H
