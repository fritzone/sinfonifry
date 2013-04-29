#ifndef SINFONIFRY_PLUGIN_WEB_H
#define SINFONIFRY_PLUGIN_WEB_H

/******************************************************************************
 * The definition of a web plugins' exposed interface towards the component   *
 * A web plugin is linked into a dynamic library and loaded upon the startup  *
 * of the application.                                                        *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"


/**
 * @defgroup group_plugins_web Web plugins
 * @{
 */

/**
 * @brief data_request is called when there is a request from the GUI to update
 * the specific plugin data of a host.
 *
 * The WEB gui of the sinfonifry presents the plugin data of each host in a
 * tabbed interface, where the tab names are the plugin names and the
 * tab data, which is placed in a @c div is returned by this function.
 *
 * This method should retrieve the entries for the given host, format an
 * html and send back (ie. return) the data to the caller. The method can use
 * the CSS styles that were provided by the styles() method.
 *
 * This method can reference javascript functions that were returned by the
 * javascripts() method below.
 *
 * @param [out] free_returned_value How to deal with the data that is
 * returned. The following values are possible:
 *   @li @c DO_NOT_FREE_ME - the caller function does not free the data.
 *   @li @c FREE_ME - free the data with the function @c free() .
 *   @li @c DELETE_ME - free the allocated memory with @c delete [] .
 * For a detailed description of these value see @link ALLOCATION_BEHAVIOR
 * @endlink
 * @param [in] ip the IP for which we request data.
 * @return an HTML string, which will be presented by the GUI in the specific
 * placeholder forthe plugin.
 */
char* data_request(ALLOCATION_BEHAVIOR* free_returned_value, const char* ip);

/**
 * @brief styles is called upon the first rendering of the container page for a host
 * which will hold the table with the data for the plugins.
 *
 * To be on the safe side we reccomend to incorporate the name of your plugin into the CSS
 * style names. Return a string which can be included in the @c <style> tag
 * of the resulting HTML page, ie. follow standard CSS syntax.
 *
 * The application will not free the returned string.
 *
 * @return the styles to be used by this plugin.
 */
const char* styles();

/**
 * @brief javascripts is called upon first rendering of the container page for a host
 * which will hold the table with the data for the plugins.
 *
 * To be on the safe side your javascript function names should contain your plugin name.
 * Here return scripts tah will be inserted into a @code <script type="text/javascript"> @endcode
 * tag in the final HTML page.
 *
 * The application does not free the returned string.
 *
 * @return the javascript statments your plugin will need.
 */
const char* javascripts();

/**
 * @brief descriptive_name returns the descriptive name of the plugin that can be used
 * in the tab.
 *
 * @return the descriptive name
 */
const char* descriptive_name();

/*****************************************************************************
 *      Typedefs used for the function pointers of client plugins            *
 *****************************************************************************/

/**
 * @}
 */

/** Function pointer typedef for the @link data_request()
 *  @endlink function. */
typedef char* (*P_WEB_DATA_REQUEST)(ALLOCATION_BEHAVIOR*, const char*);

/** Function pointer typedef for the @link styles() @endlink
 *  function */
typedef const char* (*P_WEB_STYLES)();

/** Function pointer typedef for the @link javascripts() @endlink
 *  function */
typedef const char* (*P_WEB_JAVASCRIPTS)();

/** Function pointer typedef for the @link descriptive_name() @endlink
 *  function */
typedef const char* (*P_WEB_DESCRIPTIVE_NAME)();

#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_PLUGIN_WEB_H
