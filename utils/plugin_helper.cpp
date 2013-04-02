#include "plugin_helper.h"
#include "configuration.h"
#include "components.h"
#include "utils.h"
#include "instdir.h"
#include <cxxtools/log.h>

#include <dlfcn.h>

#include <iostream>

log_define("sinfonifry.util.plugin_helper")

std::vector<plugin_descriptor*> get_plugins(PLUGIN_COMPONENT comp,
                                            const Configuration& conf)
{
    std::string comp_name = component_name_from_component_code(comp);
    std::vector<std::string> plugin_names = conf.getPlugins(comp_name);
    std::vector<plugin_descriptor*> plugins;
    for(int i=0; i<plugin_names.size(); i++)
    {
        std::string plugin_name = "libsinfonifry_"+comp_name + "_plugin_"
                + plugin_names[i];
        plugin_name += ".so";
        plugin_name = std::string(instdir) +
                "/sinfonifry/" + comp_name + "/plugins/" +
                plugin_name;
        void *lib_handle = dlopen(plugin_name.c_str(), RTLD_LAZY);
        char *error;

        if(!lib_handle)
        {
            log_error ("Cannot load plugin: " << plugin_name);
            if ((error = dlerror()) != NULL)
            {
                log_error( "Reason: " << error);
                return std::vector<plugin_descriptor*>();
            }
            return std::vector<plugin_descriptor*>();
        }


        plugin_descriptor* plugin = 0;

        if(comp == PLUGIN_CLIENT)
        {
            plugin = new client_plugin_descriptor;

            P_CLIENT_EXECUTE execmth=(P_CLIENT_EXECUTE)dlsym(lib_handle, "execute");
            if ((error = dlerror()) != NULL)
            {
                log_error("No execute() in " << plugin_name << ". " << error);
                return std::vector<plugin_descriptor*>();
            }

            static_cast<client_plugin_descriptor*>(plugin)->f_execute = execmth;
        }

        if(!plugin)
        {
            continue;
        }

        plugin->name = new char[plugin_names[i].length() + 1];
        copy_string_to_char(plugin_names[i], plugin->name);
        copy_string_to_char(plugin_name, plugin->lib_name);
        plugin->lib_handle = lib_handle;

        plugins.push_back(plugin);

    }
    return plugins;
}
