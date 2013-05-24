#ifndef SINFONIFRY_SIGNED_PLUGIN_METHOD_UNLOAD_H
#define SINFONIFRY_SIGNED_PLUGIN_METHOD_UNLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup group_plugins_signed Plugins with signature
 * @{
 */

/**
 * @brief The PLUGIN_UNLOAD_STATUS enum has codes to signal to the system the
 *        status of a plugins' unload process, when requested so by the system.
 *
 *        @li @c PLUGIN_UNLOAD_STATUS_PROCEED - the plugin succesfully unloaded
 *               itself. The system shutdown can continue as planned,
 *               INFO level log entry will come up in the log.
 *
 *        @li @c PLUGIN_UNLOAD_STATUS_TRY_AGAIN - the plugin did not unload
 *               itself yet, the systems will try for once more to unload
 *               it when all the plugins were unloaded (ie: it
 *               puts this plugin at the end of the unload
 *               queue and calls again this method). An INFO
 *               level log entry will be added to the log.
 *
 *        @li @c PLUGIN_UNLOAD_STATUS_FAILED - the plugin was not succesfully
 *               unloaded. The system will forcefully close the library
 *               hosting this plugin and add an ERROR entry in
 *               the log.
 */
enum PLUGIN_UNLOAD_STATUS
{
    PLUGIN_UNLOAD_STATUS_PROCEED     = 1,
    PLUGIN_UNLOAD_STATUS_TRY_AGAIN   = 2,
    PLUGIN_UNLOAD_STATUS_FAILED      = 3,

    PLUGIN_UNLOAD_STATUS_LAST
};

/**
 * @brief The PLUGIN_UNLOAD_REQUEST enum has the codes that are used when
 *        calling the @link unload() @endlink function
 *
 *        @li @c PLUGIN_UNLOAD_REQUEST_SHUTDOWN - the plugin is being unloaded
 *               because the sinfonifry system being shut down.
 *
 *        @li @c PLUGIN_UNLOAD_REQUEST_BY_REQUEST - the plugin is being unloaded
 *               due to a manual request from the web gui
 *
 *        @li @c PLUGIN_UNLOAD_REQUEST_LAST_WARNIG - the plugin is being
 *               unloaded as a result of a previous call of the @link unload()
 *               @endlink method which returned the @c
 *               PLUGIN_UNLOAD_STATUS_TRY_AGAIN value
 *
 */
enum PLUGIN_UNLOAD_REQUEST
{
    PLUGIN_UNLOAD_REQUEST_SHUTDOWN    = 1,
    PLUGIN_UNLOAD_REQUEST_BY_REQUEST  = 2,
    PLUGIN_UNLOAD_REQUEST_LAST_WARNIG = 3,

    PLUGIN_UNLOAD_REQUEST_LAST
};

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
 *      Typedefs used for the unload method of the signed plugins            *
 *****************************************************************************/

/** Function pointer typedef for the @link unload() @endlink function */
typedef PLUGIN_UNLOAD_STATUS (*P_UNLOAD)(PLUGIN_UNLOAD_REQUEST);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
