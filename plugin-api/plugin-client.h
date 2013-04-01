#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * The definition of a client plugins' exposed interface towards the application
 * A client plugin is linked into a dynamic library (.so) and loaded upon start
 * up of the application. The methods you implement below are called in the
 * specific stage in the life cycle of the application.
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
 *                  This method is called at regular intervals while running,
 *                  this value is specified in the "frequency" setting of the
 *                  specific plugin.
 *
 * @param free_returned_value - how to deal with the data that is returned.
 *                  The following values are possible:
 *
 *                  DO_NOT_FREE_ME - the caller function does not free the data.
 *                  FREE_ME - free the data with the function free().
 *                  DELETE_ME - free the allocated memory with delete []
 *
 * @return - the gathered data formatted for the core side component. If this
 *                  returns NULL or empty string it is ignored by the caller.
 */
char* execute(ALLOCATION_BEHAVIOR* free_returned_value);

/**
 * @brief component - returns the @see PLUGIN_COMPONENT of this plugin (ie: the
 *                    component to which the plugin belongs). 
 * 
 *                    This is just a security check, to make it possible to 
 *                    prevent the loading of non client plugins into the client.
 *
 *                    This method for the client plugins should always return
 *                    the PLUGIN_CLIENT value.
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
 *         @see PLUGIN_LOAD_STATUS for a more detailed description of the values
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
 * 
 * @return PLUGIN_UNLOAD_STATUS_PROCEED - in case of success
 *         PLUGIN_UNLOAD_STATUS_TRY_AGAIN - in case the system should try to
 *                 unload te plugin again.
 * 	       PLUGIN_UNLOAD_STATUS_FAILED - in case of a failure.
 * 
 *         @see PLUGIN_UNLOAD_STATUS for detailed description.
 *       
 *         Any other value will be treated with an ERROR level entry in the log 
 *         file and the plugin will be removed.
 */
PLUGIN_UNLOAD_STATUS unload(PLUGIN_UNLOAD_REQUEST reason);

/**
 * @brief about - Returns an "about" string, such as "Plugin (c) 2012 XYZ."
 *                The caller will NOT free the data returned from here, so
 *                usually here you should return a const static char* string.
 *
 * @return the about string of the plugin
 */
const char* about();

/**
 * @brief name - returns the name of the plugin. This name should be the same as
 *               in the configuration file. The caller of the function does not 
 *               free the returned data, so in case to avoid memory leaks return
 *               here a string which was not allocated dynamically.
 *
 * @return the name of the plugin
 */
const char* name();

/**
 * @brief signature - returns the signed identity of the plugin. Signed 
 *                    identities are allocated by the project, in order to keep
 *                    track of the plugins. Return 0 (or NULL, depending on
 *                    your system) in case of an unsigned plugin. The caller
 *                    does not free the returned data.
 * 
 *  @return the signature of the plugin.
 **/
const char* signature();

/**
 * @brief setup - this function provides internal setup functionality for the
 *                plugin. Implement this as per your requirements of the
 *                interpretation of the commands. Return any value you consider
 *                relevant to be passed on to the caller.
 *
 *                The typical workflow for this functionality is:
 *
 *                1. In the web gui the user selects the host, and there it sees
 *                   the plugins the client component of that host has.
 *                2. The web gui provides a list of the plugin specific options
 *                   the user can choose from (such as for the disk status
 *                   plugin: do not scan this hard drive for a speicifc hard
 *                   disk) and the user chooses the option
 *                3. The web-gui renders the user selected option into a
 *                   command string (such as: DO_NOT_SCAN /dev/sdb1)
 *                4. This command is sent to the client side of the
 *                   plugin triumvirate and this setup() method takes care of
 *                   the action.
 * 
 *                The setup method should not free the commands' parameter.
 *
 * @return any value you consider to be useful and which can be handled by the
 *         web gui.
 */
const char* setup(const char* commands);

/*****************************************************************************
 *             Typedefs used for the function pointers                       *
 *****************************************************************************/

/** typedef for the @see execute(ALLOCATION_BEHAVIOR*) function */
typedef char* (*P_CLIENT_EXECUTE)(ALLOCATION_BEHAVIOR*);

/** typedef for the @see setup(const char*) function */
typedef const char* (*P_CLIENT_SETUP)(const char*);

/** typedef for the @see component() function */
typedef PLUGIN_COMPONENT (*P_CLIENT_COMPONENT)();

/** typedef for the @see load() function */
typedef PLUGIN_LOAD_STATUS (*P_CLIENT_LOAD)();

/** typedef for the @see unload() function */
typedef PLUGIN_UNLOAD_STATUS (*P_CLIENT_UNLOAD)(PLUGIN_UNLOAD_REQUEST);

/** typedef for the @see about() function */
typedef const char* (*P_CLIENT_ABOUT)();

/** typedef for the @see name() function */
typedef const char* (*P_CLIENT_NAME)();

/** typedef for the @see signature() function */
typedef const char* (*P_CLIENT_SIGNATURE)();

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H
