#ifndef SINFONIFRY_SIGNED_PLUGIN_METHOD_LOAD_H
#define SINFONIFRY_SIGNED_PLUGIN_METHOD_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup group_plugins_signed Plugins with signature
 * @{
 */

/**
 * @brief The PLUGIN_LOAD_STATUS enum tells us the load status of the plugin:
 *
 *         @li @c PLUGIN_LOADED - The plugin succesfully initialized itself
 *                         and the system can continue starting normally. The
 *                         plugin will be added to the active plugins in the
 *                         system. An INFO level log entry will be created
 *                         with the name of the loaded plugin.
 *
 *         @li @c PLUGIN_LOADED_WITH_WARNINGS  - The plugin loaded, but some
 *                         warnings were recorded during the load sequence. In
 *                         this case the system will continue loading and the
 *                         pugin will be added to the active plugins in the
 *                         system and a log entry will appear with the value of
 *                         @link last_error_text() @endlink on the WARNING level
 *
 *         @li @c PLUGIN_LOAD_FAILED - The plugin failed to load. In this case
 *                         the plugin will not be added to the internal list of
 *                         active plugins and an ERROR level entry will be
 *                         added to the log file with the value of
 *                         @link last_error_text() @endlink
 *
 *        @li @c  PLUGIN_LOAD_SYSTEM_FAILURE - in this case an ERROR level entry
 *                         will be added to the log file with the value of
 *                         @link last_error_text() @endlink and the system will
 *                         interrupt its normal startup, unload all the loaded
 *                         plugins and exit.
 *
 */
enum PLUGIN_LOAD_STATUS
{
    PLUGIN_LOADED               = 1,
    PLUGIN_LOADED_WITH_WARNINGS = 2,
    PLUGIN_LOAD_FAILED          = 3,
    PLUGIN_LOAD_SYSTEM_FAILURE  = 4,

    PLUGIN_LOAD_STATUS_LAST
};

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

/*****************************************************************************
 *      Typedefs used for the load method of the signed plugins              *
 *****************************************************************************/

/** Function pointer typedef for the @link load() @endlink function */
typedef PLUGIN_LOAD_STATUS (*P_LOAD)();

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
