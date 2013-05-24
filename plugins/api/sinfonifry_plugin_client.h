#ifndef PLUGIN_CLIENT_H
#define PLUGIN_CLIENT_H

/******************************************************************************
 * The definition of a client plugins' exposed interface towards the component*
 * A client plugin is linked into a dynamic library (.so on Linux and .dll on *
 * Windows) and loaded upon the startup of the application. The methods you   *
 * implement below are called in the specific stage in the life cycle of the  *
 * application.                                                               *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"

/*****************************************************************************
 *             Methods that are exposed in a client side plugin              *
 *****************************************************************************/

/**
 * @defgroup group_plugins_client Client plugins
 * @{
 */

/**
 * @brief This function provides internal setup functionality for the plugin.
 *
 *
 * Implement this as per your requirements of the interpretation of the
 * commands. Return any value you consider relevant to be passed on to the
 * caller, ie. the web-side correspondent plugin.
 *
 * The typical workflow for this functionality is:
 *
 * -# In the web gui the user selects the host, and there it sees
 *   the plugins the client component of that host has.
 * -# The web gui provides a list of the plugin specific options
 *   the user can choose from (such as for the disk status
 *   plugin: do not scan this hard drive for a speicifc hard
 *   disk) and the user chooses the option
 * -# The web-gui renders the user selected option into a
 *   command string (such as: DO_NOT_SCAN /dev/sdb1)
 * -# This command is sent to the client side of the
 *   plugin triumvirate and this setup() method takes care of
 *   the action.
 *
 * The setup method should not free the commands' parameter.
 *
 * @return any value you consider to be useful and which can be handled by the
 *         corresponding web side plugin.
 */
const char* setup(const char* commands);

/*****************************************************************************
 *      Typedefs used for the function pointers of client plugins            *
 *****************************************************************************/

/** Function pointer typedef for the @link setup(const char*) @endlink
 *  function */
typedef const char* (*P_CLIENT_SETUP)(const char*);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_CLIENT_H
