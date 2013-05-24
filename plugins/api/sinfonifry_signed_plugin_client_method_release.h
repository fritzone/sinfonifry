#ifndef SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_RELEASE_H
#define SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_RELEASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup group_client_plugins_signed Client plugins with valid signature
 * @{
 */
/**
 * @brief This method is called after the value returned by execute() was
 * processed by the framework.
 *
 * @param [in] intermediary is the pointer that was returned by the
 * execute() method. Please deal with it using the appropriate methods for your
 * plugin.
 */
void release(char* intermediary);

/*****************************************************************************
 *      Typedefs used for the release method of a sigedn client plugin       *
 *****************************************************************************/

/** Function pointer typedef for the @link release(char*) @endlink
 *  function */
typedef void (*P_CLIENT_RELEASE)(const char*);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // SINFONIFRY_SIGNED_PLUGIN_CLIENT_METHOD_RELEASE_H
