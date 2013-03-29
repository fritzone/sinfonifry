#ifndef PLUGIN_HELPER_H
#define PLUGIN_HELPER_H

#include <vector>
#include "plugin-client.h"

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

    // the function pointer which returns the current data of the plugin

    PEXECUTE execute;

};


std::vector<plugin_descriptor*> get_plugins(PLUGIN_COMPONENT,
                                            const Configuration &conf);

#endif // PLUGIN_HELPER_H
