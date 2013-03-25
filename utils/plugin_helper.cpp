#include "plugin_helper.h"
#include "configuration.h"
#include "components.h"
#include "utils.h"

#include <dlfcn.h>

#include <iostream>

std::vector<plugin_descriptor*> get_plugins(PLUGIN_COMPONENT comp,
                                            const Configuration& conf)
{
    std::string comp_name = component_name_from_component_code(comp);
    std::vector<std::string> plugin_names = conf.getPlugins(comp_name);
    std::vector<plugin_descriptor*> plugins;
    for(int i=0; i<plugin_names.size(); i++)
    {
        std::string plugin_name = "libsinfonifry_client_plugin_" + plugin_names[i];
        plugin_name += ".so";
        plugin_name = "/opt/sinfonifry/client/plugins/" + plugin_name;
        void *lib_handle = dlopen(plugin_name.c_str(), RTLD_LAZY);
        if(!lib_handle)
        {
            std::cerr << "Cannot load plugin: "<<plugin_name <<std::endl;
            return std::vector<plugin_descriptor*>();
        }

        PEXECUTE execmth = (PEXECUTE)dlsym(lib_handle, "execute");
        char *error;
        if ((error = dlerror()) != NULL)
        {
            std::cerr << "No execute() in " << plugin_name << ". " << error;
            return std::vector<plugin_descriptor*>();
        }

        plugin_descriptor* plugin = new plugin_descriptor;
        plugin->name = new char[plugin_names[i].length() + 1];
        copy_string_to_char(plugin_names[i], plugin->name);
        copy_string_to_char(plugin_name, plugin->lib_name);
        plugin->lib_handle = lib_handle;
        plugin->execute = execmth;

        plugins.push_back(plugin);

    }
    return plugins;
}
