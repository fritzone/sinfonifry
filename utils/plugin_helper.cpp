#include "plugin_helper.h"
#include "configuration.h"
#include "components.h"
#include "utils.h"
#include "instdir.h"
#include <cxxtools/log.h>

#include <dlfcn.h>

#include <iostream>

log_define("sinfonifry.util.plugin_helper")

namespace sinfonifry
{

template <class T>
T plugin_descriptor::get_method(const char* mth)
{
    T m = (T)dlsym(lib_handle, mth);
    char* error;
    if ((error = dlerror()) != NULL)
    {
        log_error("No " << mth << " in " << name << ". " << error);
        return 0;
    }
    return m;
}

void PluginHelper::discard(plugin_descriptor*& plugin, const char *msg)
{
    log_error("Discarding plugin " << plugin->name << " because: " << msg);
    dlclose(plugin->lib_handle);
    free(plugin->name); // this is allocated with calloc in copy_string_to_char
    free(plugin->lib_name); // same
    delete plugin;
}

PluginHelper::PluginHelper() : m_forbiddenPlugins()
{}

void PluginHelper::addForbiddenPlugin(const std::string &name, const std::string &reason)
{
    m_forbiddenPlugins.insert(std::make_pair<std::string, std::string>(name, reason));
}

bool PluginHelper::checkSignature(const char*, const char*)
{
    return true;
}

PluginHelper& PluginHelper::instance()
{
    static PluginHelper t_instance;
    return t_instance;
}

std::vector<plugin_descriptor*> PluginHelper::getSignedPlugins(PLUGIN_COMPONENT comp, const Configuration* conf, const char* signing_authority)
{
    std::string comp_name = sinfonifry::component_name_from_component_code(comp);
    std::vector<std::string> plugin_names = conf->getPlugins(comp_name);
    std::vector<plugin_descriptor*> plugins;
    for(int i=0; i<plugin_names.size(); i++)
    {
        std::string plugin_name_library = "libsinfonifry_" + comp_name + "_plugin_" + plugin_names[i];
        plugin_name_library += ".so";
        plugin_name_library = std::string(instdir) + "/sinfonifry/" + comp_name + "/plugins/" + plugin_name_library;

        // open the library
        void *lib_handle = dlopen(plugin_name_library.c_str(), RTLD_LAZY);
        char *error = 0;

        if(!lib_handle)
        {
            log_error ("Cannot load plugin: " << plugin_name_library);
            if ((error = dlerror()) != NULL)
            {
                log_error( "Reason: " << error);
            }
            dlclose(lib_handle);
            continue;
        }


        plugin_descriptor* plugin = new client_plugin_descriptor;
        plugin->lib_handle = lib_handle;

        copy_string_to_char(plugin_names[i], plugin->name);
        copy_string_to_char(plugin_name_library, plugin->lib_name);

        // first steps: load the common methods

        // load the component() method
        if(!(plugin->f_component = plugin->get_method<P_COMPONENT>("component")))
        {
            discard(plugin, "no component() method");
            continue;
        }

        // see if this is a matching side plugin or not
        PLUGIN_COMPONENT t_comp = plugin->f_component();
        if(t_comp != comp)
        {
            discard(plugin, "misplaced plugin (wrong component)");
            log_error("plugin suggested he is [" << component_name_from_component_code(t_comp)
                      << "] and we are expecting it to be [" << component_name_from_component_code(comp)
                      << "]. Fix the configuration file, this plugin is in the wrong location."
                      );
            continue;
        }

        // load the signature method
        if(!(plugin->f_signature = plugin->get_method<P_SIGNATURE>("signature")))
        {
            discard(plugin, "no signature() method");
            continue;
        }

        // check that this is a signed plugin
        const char* signature = plugin->f_signature();
        bool plugin_with_signature = checkSignature(signature, signing_authority);
        if(!plugin_with_signature)
        {
            discard(plugin, "not signed plugin");
            continue;
        }

        // check the name() method and that it returns valid data
        if(!(plugin->f_name == plugin->get_method<P_NAME>("name")))
        {
            discard(plugin, "no name() method");
            continue;
        }
        const char* name = plugin->f_name();
        if(strcmp(name, plugin->name))
        {
            discard(plugin, "inconsistent name for the plugin");
            log_error("Plugin returned [" << name << "] while expecting [" << plugin->name << "]. Fix your plugin.");
            continue;
        }

        // check the about() method
        if(!(plugin->f_about == plugin->get_method<P_ABOUT>("about")))
        {
            log_warn("No about() method in " << plugin->name << ". Consider implementing one.");
        }

        // now load the component specific methods
        if(comp == PLUGIN_CLIENT)
        {
            // load the execute method
            P_CLIENT_EXECUTE execmth = plugin->get_method<P_CLIENT_EXECUTE>("execute");
            if (execmth == 0)
            {
                discard(plugin, "no execute() method for a client plugin");
                continue;
            }
            static_cast<client_plugin_descriptor*>(plugin)->f_execute = execmth;
        }

        // we are here, this plugin seems to be a happy one
        log_info("plugin " <<plugin->name << " loaded from " << plugin->lib_name);
        plugins.push_back(plugin);

    }
    return plugins;
}
}
