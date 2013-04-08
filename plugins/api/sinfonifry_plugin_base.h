#ifndef SINFONIFRY_PLUGIN_BASE_H
#define SINFONIFRY_PLUGIN_BASE_H

/******************************************************************************
 * This header contains the method declarations that are to be shared by all  *
 * the plugins of the sinfonifry system, regardless of the component and the  *
 * signedness of the plugin.                                                  *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"
#include "sinfonifry_errors.h"

/*****************************************************************************
 *          Methods that are exposed by all the sinfonifry plugins           *
 *****************************************************************************/

/**
 * @defgroup group_plugins_unsigned Unsigned plugins
 * @{
 */

/**
 * @brief Retrieve the component to which this plugin belongs.
 *
 * This method returns the @link PLUGIN_COMPONENT @endlink of this plugin
 * (ie: the component to which the plugin belongs). This implements a security
 * check,to make it possible to prevent the loading of non client plugins into
 * the client. This method f or the client plugins should always return the
 * PLUGIN_CLIENT value. If this method returns invalid data the plugin will
 * not be loaded in the client component.
 *
 * This method is called both for signed and unsigned plugins.
 *
 * @return The @link PLUGIN_COMPONENT @endlink of this plugin.
 *
 * @see @c PLUGIN_COMPONENT for a description of the values
 */
PLUGIN_COMPONENT component();

/**
 * @brief Returns an "about" string, such as "Plugin (c) 2012 XYZ."
 *
 * The caller will NOT free the data returned from here, so
 * usually here you should return a @c const @c static @c char* string.
 *
 * This method is called both for signed and unsigned plugins.
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
 * This method is called both for signed and unsigned plugins.
 *
 * @return The name of the plugin.
 */
const char* name();

/**
 * @brief Returns the signed identity of the plugin.
 *
 * Signed identities are allocated by the project, in order to keep track of
 * the plugins. Return @c 0 (or @c NULL, depending on your system) in case of an
 * unsigned plugin.
 *
 * Plugins which do not have a (valid) signature will be loaded via the plugin
 * wrapper component, plugins with a valid signature are considered to be
 * trusted plugins, they are loaded internally.
 *
 * The caller does not free the returned data.
 *
 * @return The signature of the plugin.
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
 * This method is called both for signed and unsigned plugins.
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
 * This method is called both for signed and unsigned plugins.
 *
 * @return The last error.
 */
const char* last_error_text();

/*****************************************************************************
 *      Typedefs used for the function pointers of all the plugins           *
 *****************************************************************************/

/** Function pointer typedef for the @link component() @endlink function */
typedef PLUGIN_COMPONENT (*P_COMPONENT)();

/** Function pointer typedef for the @link about() @endlink function */
typedef const char* (*P_ABOUT)();

/** Function pointer typedef for the @link name() @endlink function */
typedef const char* (*P_NAME)();

/** Function pointer typedef for the @link signature() @endlink function */
typedef const char* (*P_SIGNATURE)();

/**
 * @}
 **/

#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_PLUGIN_BASE_H
