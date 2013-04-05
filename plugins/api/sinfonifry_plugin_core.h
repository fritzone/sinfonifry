#ifndef PLUGIN_CORE_H
#define PLUGIN_CORE_H

/******************************************************************************
 * The definition of a core plugins' exposed interface towards the component  *
 * A ccore plugin is linked into a dynamic library (.so on Linux and .dll on  *
 * Windows) and loaded upon the startup of the application. The methods you   *
 * implement below are called in the specific stage in the life cycle of the  *
 * application.                                                               *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "sinfonifry_plugin_base.h"
#include "sinfonifry_errors.h"

/**
 * @brief Called when the corresponding client side plugin sends data.
 *
 * The data is prepared by the client side plugin in a manner to be understood
 * by this method.
 * 
 * The data is NOT encapsulated in any kind of wrapper, what the client side
 * plugin sent here is received integrally. The functions should not free
 * the @c data since it is managed by the caller.
 * 
 * @param data The data that was gathered by the client side plugin.
 * 
 * @return 0 in case of error, then the @c last_error() is called, 
 * there will be an entry in the log. Any other value is ignored by the 
 * caller, thus considered to be a succesfull completion.
 */
int data_received(const char* data);

/**
 * @brief Called if the plugin needs to initialize internal data for the host.
 * 
 * This method is called for each host that was added to the system in the 
 * following circumstances
 * 
 * -# The host was added into the sinfonifry system via the web gui.
 * -# The host was requested a re-initialization from the web gui.
 * 
 * To understand this more, you need to understand how the sinfonifry system 
 * works. The typical workflow regarding this is the following: 
 * 
 * -# The Host is added to the system or marked for reinitialization
 * -# The host is placed in the @c sinf01_allowed_hosts table meaning, this
 *    host is allowed to connect to the system from now on.
 * -# The IP of the host is placed inside the @c sinf02_host_init table,
 *    meaning the plugins need to initialize their internal structures
 *    to accomodate this plugin. 
 * -# The newly installed client on the host that was just added sends the 
 *    first set of gathered data to the core, and the sinfonifry core system
 *    checks to see if the host is in the @c sinf02_host_init table, in case
 *    it is there this method is called.
 * 
 * A host is usually marked for reinitialization when its hardware changes,
 * such as a new harddisk is added.
 * 
 * @param host_ip The IP of the host which is initialized. This is supposed
 * to be unique across the network, so is used as primary key. Do not attempt
 * to free this data.
 * @param data This is the same data that was sent by the plugin. When the
 * @c initialize_host() call is completed the @c data_received() method will be 
 * called with the same data. Do not attempt to free this data.
 * 
 * @return 0 in case of error, in this case the @c last_error() method will be
 * called and a log entry will be created. Any other value is ignored.
 */
int initialize_host(const char* host_ip, const char* data);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_CORE_H
