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
 * tab data (which is placed in a @c div named @c div_name ) is returned by this
 * function.
 *
 * This method should retrieve the entries for the given host, format an
 * html and send back (ie. return) the data to the caller. The method can use
 * the CSS styles that were provided by the styles() method.
 *
 * This method can reference javascript functions that were returned by the
 * javascripts() method below.
 *
 * The final destination for this data is inside the @c div whose name is passed
 * in as a parameter to this function (@c div_name). This will allow you to
 * write more dynamic HTML.
 *
 * @b Links
 *
 * In order to have a consistency among the way the results are shown there is
 * a link filtering in the application. Every @c "a" HTML tag is being parsed
 * from the returned string, and rewritten in a way to display the result from
 * the server in the @c div dedicated to the plugin.
 *
 * The linking functionality has been reduced to make a request via an ajax
 * based interface and to display the reply from the server, linking to external
 * sites is not working, unless thy can send reply in a manner to be understood
 * by the ajax mechanism of sinfonifry.
 *
 * @b Entrypoints
 *
 * In order to support the entrypoints the following HTML extension was added
 * to the application:
 *
 * @code
 * <@ep name="ep_name" params="id=1234" display="link_text">
 * @endcode
 *
 * When encountered this extension sequence in the returned code the sinfonifry
 * parser will generate a valid @c a HTML tag for the extension point manager
 * to call the required extension point in the © div of the plugin.
 *
 * @param [out] free_returned_value How to deal with the data that is
 * returned. The following values are possible:
 *   @li @c DO_NOT_FREE_ME - the caller function does not free the data.
 *   @li @c FREE_ME - free the data with the function @c free() .
 *   @li @c DELETE_ME - free the allocated memory with @c delete [] .
 * For a detailed description of these value see @link ALLOCATION_BEHAVIOR
 * @endlink
 * @param [in] ip the IP for which we request data.
 * @param [in] div_name The name of the @c div tag into which the generated HTML will go.
 * @return an HTML string, which will be presented by the GUI in the specific
 * placeholder forthe plugin.
 */
char* data_request(ALLOCATION_BEHAVIOR* free_returned_value, const char* ip, const char* div_name);

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

/**
 * @brief entrypoints should return a vector of enrtypoints which can be referenced from the
 * generated script.
 *
 * The last element of this array acts as a sentinel and is always 0. If the plugin loader
 * encounters an entrypoint which cannot be loaded from the library it will automatically
 * discard the plugin.
 *
 * An entrypoint is a function with signature @code void entrypoint(const char* query_parameters)
 * @endcode . The query_parameters is the request parameters passed in to the function-
 *  In order to retrieve the values of the query parameters use the query_param() function.
 * @return
 */
const char* const* entrypoints();


/*****************************************************************************
 *      Typedefs used for the function pointers of client plugins            *
 *****************************************************************************/

/**
 * @}
 */

/** Function pointer typedef for the @link data_request()
 *  @endlink function. */
typedef char* (*P_WEB_DATA_REQUEST)(ALLOCATION_BEHAVIOR*, const char*, const char*);

/** Function pointer typedef for the @link styles() @endlink
 *  function */
typedef const char* (*P_WEB_STYLES)();

/** Function pointer typedef for the @link javascripts() @endlink
 *  function */
typedef const char* (*P_WEB_JAVASCRIPTS)();

/** Function pointer typedef for the @link descriptive_name() @endlink
 *  function */
typedef const char* (*P_WEB_DESCRIPTIVE_NAME)();

/** Function pointer typedef for the @link entrypoints() @endlink
 *  function */
typedef const char* const *(*P_WEB_ENTRYPOINTS)(const char*);

#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_PLUGIN_WEB_H
