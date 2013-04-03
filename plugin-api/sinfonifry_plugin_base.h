#ifndef SINFONIFRY_PLUGIN_BASE_H
#define SINFONIFRY_PLUGIN_BASE_H

/******************************************************************************
 * This header contains the method declarations that are to be shared by all  *
 * the plugins of the sinfonifry system, regardless of the component.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"
#include "sinfonifry_errors.h"

/*****************************************************************************
 *          Methods that are exposed by all the sinfonifry plugins            *
 *****************************************************************************/

/**
 * @brief Retrieve the component to which this plugin belongs
 *
 * This method returns the @link @c PLUGIN_COMPONENT @endlink of this plugin
 * (ie: the component to which the plugin belongs). This implements a security
 * check,to make it possible to prevent the loading of non client plugins into
 * the client. This method f or the client plugins should always return the
 * PLUGIN_CLIENT value. If this method returns invalid data the plugin will
 * not be loaded in the client component.
 *
 * @return The @link @c PLUGIN_COMPONENT @endlink of this plugin.
 *
 * @see @c PLUGIN_COMPONENT for a description of the values
 */
PLUGIN_COMPONENT component();

/**
 * @brief This method is called when the component loads the plugin.
 *
 * The method is called when the system loads this plugin. Do one time
 * initialization here, and return the correct status. The resources you
 * allocate in this method can be freed in the @link unload() @endlink.
 *
 * @return The corresponding status of the plugin, such as:
 *         @li @c PLUGIN_LOADED - in case of success.
 *         @li @c PLUGIN_LOADED_WITH_WARNINGS  - in case the plugin loaded, but
 *                some warnings were recorded during the load sequence.
 *         @li @c PLUGIN_LOAD_FAILED - in case the plugin failed to load.
 *         @li @c PLUGIN_LOAD_SYSTEM_FAILURE - in case the system cannot
 *                continue loading.
 *
 *  Any other value will be treated with an ERROR level entry in the log
 *  file and the plugin will not be added to the loaded plugins (ie: it
 *  will be unloaded).
 *
 *  @see @c PLUGIN_LOAD_STATUS for a more detailed description of the values
 */
PLUGIN_LOAD_STATUS load();

/**
 * @brief Called when the plugin is about to be unloaded from the component.
 *
 * This method is called either when the sinfonifry system shuts down, and all
 * the plugins are unloaded or when there was a specific request for the plugin
 * to be unloaded from the system (initiated from the web gui).
 *
 * @param [in] reason Tells us why the plugin is being unloaded.
 *
 * @return @li @c PLUGIN_UNLOAD_STATUS_PROCEED - in case of success
 *         @li @c PLUGIN_UNLOAD_STATUS_TRY_AGAIN - in case the system should try
 *                to unload te plugin again.
 * 	       @li @c PLUGIN_UNLOAD_STATUS_FAILED - in case of a failure.
 *
 *         Any other value will be treated with an ERROR level entry in the log
 *         file and the plugin will be removed.
 *
 * @see @c PLUGIN_UNLOAD_STATUS for detailed description of the values you can
 *      return.
 * @see @c PLUGIN_UNLOAD_REQUEST for a detailed description of the request why
 *      your plugin is unloaded.
 */
PLUGIN_UNLOAD_STATUS unload(PLUGIN_UNLOAD_REQUEST reason);

/**
 * @brief Returns an "about" string, such as "Plugin (c) 2012 XYZ."
 *
 * The caller will NOT free the data returned from here, so
 * usually here you should return a @c const @c static @c char* string.
 *
 * @return The about string of the plugin, @c NULL or empty string are being
 *         ignored.
 */
const char* about();

/**
 * @brief Returns the name of the plugin.
 *
 * This name should be the same as in the configuration file. The caller of the
 * function does not free the returned data, so in case to avoid memory leaks
 * return here a string which was not allocated dynamically.
 *
 * @return The name of the plugin.
 */
const char* name();

/**
 * @brief Returns the signed identity of the plugin.
 *
 * Signed identities are allocated by the project, in order to keep track of
 * the plugins. Return @c 0 (or @c NULL, depending on your system) in case of an
 * unsigned plugin. The caller does not free the returned data.
 *
 *  @return The signature of the plugin.
 **/
const char* signature();

/**
 * @brief Return the last error.
 *
 * Returns the last error. For a detailed description of the common error codes
 * together with their meaning please consult the error documentation, otherwise
 * the plugin is responsible for providing error codes that start after the
 * value @c SINFONIFRY_LAST_ERROR .
 *
 * @return The last error.
 */
int last_error();

/**
 * @brief Return the last error text.
 *
 * Returns the last error in a human readable form. The user should not free the
 * returned data.
 *
 * @return The last error.
 */
const char* last_error_text();

/*****************************************************************************
 *      Typedefs used for the function pointers of all the plugins           *
 *****************************************************************************/

/** Function pointer typedef for the @link component() @endlink function */
typedef PLUGIN_COMPONENT (*P_COMPONENT)();

/** Function pointer typedef for the @link load() @endlink function */
typedef PLUGIN_LOAD_STATUS (*P_LOAD)();

/** Function pointer typedef for the @link unload() @endlink function */
typedef PLUGIN_UNLOAD_STATUS (*P_UNLOAD)(PLUGIN_UNLOAD_REQUEST);

/** Function pointer typedef for the @link about() @endlink function */
typedef const char* (*P_ABOUT)();

/** Function pointer typedef for the @link name() @endlink function */
typedef const char* (*P_NAME)();

/** Function pointer typedef for the @link signature() @endlink function */
typedef const char* (*P_SIGNATURE)();

#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_PLUGIN_BASE_H
