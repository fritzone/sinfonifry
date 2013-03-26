#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * The definition of a plugins' exposed interface towards the application. This
 * header file contains all the methods that the plugins of the components
 * should expose to the outside world, but you need to implement only the
 * methods that are relevant to the plugin you develop.
 *
 * For Client plugins the following methods should be implemented:
 *
 *  1. @see int load();
 *
 *  2. @see PLUGIN_COMPONENT component();
 *     Always should return PLUGIN_CLIENT
 *
 *  3. @see const char* execute();
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"

/**
 * @brief execute - this is the method which does the real work of the
 *                  plugin, (ie: it gathers the data this plugin is supposed
 *                  to send back to the server, formats it in a way that the
 *                  core components plugin understands it and returns it).
 *
 *                  The returned data will be rendered in the response XML under
 *                  the "<plugin_data>" node, inside the plugin element, like:
 *                  <plugin name=@see name()>
 *                    ... HERE GOES THE RETURN VALUE OF THE execute() METHOD ...
 *                  </plugin>
  *
 * @param free_returned_value - how to deal with the data that is returned.
 *                  The following values are possible:
 *
 *                  DO_NOT_FREE_ME - the caller function does not free the data.
 *                  FREE_ME - free the data with the function free().
 *                  DELETE_ME - free the allocated memory with delete []
 *
 * @return - the gathered data formatted for the core side component. If this
 *           return NULL or empty string it is ignored by the caller.
 */
char* execute(ALLOCATION_BEHAVIOR* free_returned_value);

/**
 * @brief component - returns the @see PLUGIN_COMPONENT of this plugin (ie: the
 *        type of the plugin).
 *
 * @return the @see PLUGIN_COMPONENT of this plugin
 */
PLUGIN_COMPONENT component();

/**
 * @brief load - called when the system loads this plugin. Do one time
 *               initialization here, and return the correct status.
 *
 * @return PLUGIN_LOADED - in case of success.
 *         PLUGIN_LOADED_WITH_WARNINGS  - in case the plugin loaded, but some
 *                         warnings were recorded during the load sequence.
 *         PLUGIN_LOAD_FAILED - in case the plugin failed to load.
 *         PLUGIN_LOAD_SYSTEM_FAILURE - in case the system cannot continue
 *                         loading.
 *
 *         Any other value will be treated with an ERROR level entry in the log
 *         file and the plugin will not be added to the loaded plugins (ie: it
 *         will be unloaded).
 */
PLUGIN_LOAD_STATUS load();

/**
 * @brief unload - This method is called either when the sinfonifry system shuts
 *                 down, and all the plugins are unloaded or when there was a
 *                 specific request for the plugin to be unloaded from the
 *                 system (initiated from the web gui)
 *
 * @param reason - tells us why the plugin is being unloaded.
 */
PLUGIN_UNLOAD_STATUS unload(PLUGIN_UNLOAD_REQUEST reason);

// an internal about string. User should NOT free the data returned from here
const char* about();

// the name of the plugin. User should NOT free this
const char* name();

// does some setup work. Plugin dependent
int setup(const char*);

/*****************************************************************************
 *             Typedefs used for the function pointers                       *
 *****************************************************************************/

/* typedef for the execute(ALLOCATION_BEHAVIOR*) function */
typedef char* (*PEXECUTE)(ALLOCATION_BEHAVIOR*);


/*
 * Data below is used by the components to identify the plugin structure
 */

/**
 * @brief The plugin struct represents
 */
struct plugin_descriptor
{
    // the name of the plugin
    char* name;

    // the name of the library this plugin resides in
    char* lib_name;

    // the handle of the library
    void* lib_handle;

    // the function pointer which returns the current data of the plugin

    PEXECUTE execute;

};


#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H
