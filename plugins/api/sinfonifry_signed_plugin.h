#ifndef SINFONIFRY_SIGNED_PLUGIN_H
#define SINFONIFRY_SIGNED_PLUGIN_H

/******************************************************************************
 * This header contains the method declarations that are to be shared by all  *
 * the signed plugins of the sinfonifry system, regardless of the component.  *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"

/**
 * @defgroup group_plugins_signed Plugins with signature
 * @{
 */

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
 *             @li @c PLUGIN_UNLOAD_STATUS_FAILED - in case of a failure.
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

/*****************************************************************************
 *      Typedefs used for the function pointers of the signed plugins        *
 *****************************************************************************/

/** Function pointer typedef for the @link load() @endlink function */
typedef PLUGIN_LOAD_STATUS (*P_LOAD)();

/** Function pointer typedef for the @link unload() @endlink function */
typedef PLUGIN_UNLOAD_STATUS (*P_UNLOAD)(PLUGIN_UNLOAD_REQUEST);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif
