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

#include "sinfonifry_plugin_base.h"
#include "sinfonifry_errors.h"

/*****************************************************************************
 *             Methods that are exposed in a client side plugin              *
 *****************************************************************************/

/**
 * @brief This is the method which does the real work of the client plugin.
 *
 * It gathers the data this plugin is supposed to send back to the server,
 * formats it in a way that the core components corresponding plugin understands
 * it and returns it.
 *
 * The returned data will be rendered in the response XML unde  the @c
 * <plugin_data> node, inside the plugin element, like:
 * @code
 * <plugin name="plugin_name">
 *  ... HERE GOES THE RETURN VALUE OF THE execute() METHOD ...
 * </plugin>
 * @endcode
 * This method is called at regular intervals while running, this value is
 * specified in the "frequency" setting of the specific plugin. For the values
 * the "frequency" setting can take please refer to the configuration doc.
 *
 * @param [out] free_returned_value How to deal with the data that is
 * returned. The following values are possible:
 *   @li @c DO_NOT_FREE_ME - the caller function does not free the data.
 *   @li @c FREE_ME - free the data with the function @c free() .
 *   @li @c DELETE_ME - free the allocated memory with @c delete [] .
 * For a detailed description of these value see @link ALLOCATION_BEHAVIOR
 * @endlink
 *
 * @return The gathered data formatted for the core side component. If this
 *                  returns @c NULL or empty string it is ignored by the caller.
 *
 * @see @c ALLOCATION_BEHAVIOR for a detailed description of the values
 */
char* execute(ALLOCATION_BEHAVIOR* free_returned_value);

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
 *         web gui.
 */
const char* setup(const char* commands);

/*****************************************************************************
 *      Typedefs used for the function pointers of client plugins            *
 *****************************************************************************/

/** Function pointer typedef for the @link execute(ALLOCATION_BEHAVIOR*)
 *  @endlink function. */
typedef char* (*P_CLIENT_EXECUTE)(ALLOCATION_BEHAVIOR*);

/** Function pointer typedef for the @link setup(const char*) @endlink
 *  function */
typedef const char* (*P_CLIENT_SETUP)(const char*);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_CLIENT_H
