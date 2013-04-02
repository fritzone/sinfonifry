#ifndef PLUGIN_HELPER_H
#define PLUGIN_HELPER_H

#include <vector>
#include "sinfonifry_plugin_client.h"

class Configuration;

/*
 * Data below is used by the components to identify the plugin structure
 */

/**
 * @brief The plugin struct represents
 */
struct plugin_descriptor
{
    // the name of the plugin
    char* name;

    // the name of the library this plugin resides in
    char* lib_name;

    // the handle of the library
    void* lib_handle;

    P_COMPONENT f_component;

    P_LOAD f_load;

    P_UNLOAD f_unload;

    P_ABOUT f_about;

    P_NAME f_name;

    P_SIGNATURE f_signature;
};

struct client_plugin_descriptor : public plugin_descriptor
{
    P_CLIENT_EXECUTE f_execute;

    P_CLIENT_SETUP f_setup;
};


std::vector<plugin_descriptor*> get_plugins(PLUGIN_COMPONENT,
                                            const Configuration &conf);

#endif // PLUGIN_HELPER_H
