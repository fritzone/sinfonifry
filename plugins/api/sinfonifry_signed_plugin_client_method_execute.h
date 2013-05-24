#ifndef SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_EXECUTE_H
#define SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_EXECUTE_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup group_client_plugins_signed Client plugins with valid signature
 * @{
 */

/**
 * @brief This is the method which does the real work of a signed client plugin.
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
 * @param [out] length will contain the length of the returned string. This
 * is especially useful if you return binary data which might contain the
 * '\0' character. If this value is 0 the \c strlen() function is used to
 * determine the length of the returned value.
 *
 * @return The gathered data formatted for the core side component. If this
 * returns @c NULL or empty string it is ignored by the caller. Expect a call
 * to the release() method with the same pointer you have returned in order to
 * manage your memory.
 */

char* execute(unsigned int* length);

/*****************************************************************************
 *      Typedefs used for the execute method of signed client plugins        *
 *****************************************************************************/

/** Function pointer typedef for the @link execute(ALLOCATION_BEHAVIOR*)
 *  @endlink function. */
typedef char* (*P_CLIENT_EXECUTE)(unsigned int*);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_EXECUTE_H
