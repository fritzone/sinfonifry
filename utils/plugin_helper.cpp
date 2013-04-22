#include "plugin_helper.h"
#include "configuration.h"
#include "components.h"
#include "utils.h"
#include "instdir.h"
#include <cxxtools/log.h>
#include <libb64/b64.h>

#include <dlfcn.h>

#include <iostream>

log_define("sinfonifry.util.plugin_helper")

namespace sinfonifry
{

template <class T>
T get_method(const char* mth, void *lib_handle)
{
    T m = (T)dlsym(lib_handle, mth);
    char* error;
    if ((error = dlerror()) != NULL)
    {
        log_error("No method [" << mth << "()] in " << name << ". " << error);
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
    free(plugin);
}

PluginHelper::PluginHelper() : m_forbiddenPlugins()
{}

void PluginHelper::addForbiddenPlugin(const std::string &name, const std::string &reason)
{
    m_forbiddenPlugins.insert(std::make_pair<std::string, std::string>(name, reason));
}

bool PluginHelper::checkSignature(const char* signature, const char* signing_authority)
{
    log_debug("checking validity of signature: " << signature);
    return true;
}

PluginHelper& PluginHelper::instance()
{
    static PluginHelper t_instance;
    return t_instance;
}

bool PluginHelper::loadCommonPlugin(const char* signing_authority, plugin_descriptor*& plugin, PLUGIN_COMPONENT comp, std::string comp_name, const Configuration* conf)
{
    // load the component() method
    if(!(plugin->f_component = get_method<P_COMPONENT>("component", plugin->lib_handle)))
    {
        discard(plugin, "no component() method. This method is mandatory for any plugin.");
        return false;
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
        return false;
    }

    // load the signature method
    if(!(plugin->f_signature = get_method<P_SIGNATURE>("signature", plugin->lib_handle)))
    {
        log_warn("no signature() method. Checking config.");
        std::string signature_requested = conf->getConfigSettingForPlugin(comp_name, plugin->name, "provides_signature", "1");
        if(signature_requested == "1")
        {
            discard(plugin, "no signature() method, however it was required in the configuration");
            return false;
        }
    }

    // check that this is a signed plugin
    if(plugin->f_signature)
    {
        // yes, see the signature from the library itself
        const char* signature = plugin->f_signature();

        // is this a valid signature?
        bool plugin_signature_valid = checkSignature(signature, signing_authority);

        // not a valid signature
        if(!plugin_signature_valid)
        {
            discard(plugin, "the plugin provided signature is not valid");
            return false;
        }
    }
    else
    {
        log_warn("no signature() method. Getting signature from config.");
        std::string signature = conf->getConfigSettingForPlugin(comp_name, plugin->name, "signature", "");
        bool plugin_signature_valid = checkSignature(signature.c_str(), signing_authority);
        if(!plugin_signature_valid)
        {
            discard(plugin, "the config provided hardcoded signature is not valid");
            return false;
        }
        log_info("signature for " << plugin->name << " verified and found to be correct");
    }

    // check the name() method and that it returns valid data
    if(!(plugin->f_name = get_method<P_NAME>("name", plugin->lib_handle)))
    {
        // see if this plugin provides a name() method
        std::string name_requested = conf->getConfigSettingForPlugin(comp_name, plugin->name, "provides_name", "1");
        if(name_requested == "1")
        {
            discard(plugin, "no name() method, however it was required in the configuration");
            return false;
        }
        else
        {
            log_warn("This plugin provides no name() method. Consider implementing one.");
        }
    }
    else
    {
        const char* name = plugin->f_name();
        if(strcmp(name, plugin->name))
        {
            discard(plugin, "inconsistent name for the plugin");
            log_error("Plugin returned [" << name << "] while expecting [" << plugin->name << "]. Fix your plugin.");
            return false;
        }
    }

    // check the about() method
    if(!(plugin->f_about = get_method<P_ABOUT>("about", plugin->lib_handle)))
    {
        log_warn("No about() method in " << plugin->name << ". Consider implementing one.");
    }

    // check the last_error method
    if(!(plugin->f_last_error= get_method<P_LAST_ERROR>("last_error", plugin->lib_handle)))
    {
        log_warn("No last_error() method in " << plugin->name << ". This plugin will not report errors.");
    }

    return true;
}

std::vector<plugin_descriptor*> PluginHelper::getSignedPlugins(PLUGIN_COMPONENT comp, const Configuration* conf, const char* signing_authority)
{
    std::string comp_name = sinfonifry::component_name_from_component_code(comp);
    std::vector<std::string> plugin_names = conf->getPlugins(comp_name);
    std::vector<plugin_descriptor*> plugins;
    for(int i=0; i<plugin_names.size(); i++)
    {
        std::string plugin_name_library = conf->getConfigSettingForPlugin(comp_name, plugin_names[i], "library", "");
        if(plugin_name_library.empty())
        {
            log_warn("Cannot get the library name from the config file. Trying to guess.");
            plugin_name_library = "libsinfonifry_" + comp_name + "_plugin_" + plugin_names[i];
            plugin_name_library += ".so";
        }
        plugin_name_library = std::string(instdir) + "/sinfonifry/" + comp_name + "/plugins/" + plugin_name_library;
        log_debug("Opening " << comp_name << " plugin from " << plugin_name_library);

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

        // create a basic plugin object
        plugin_descriptor* plugin = (plugin_descriptor*)calloc(sizeof(plugin_descriptor), 1);
        plugin->lib_handle = lib_handle;
        copy_string_to_char(plugin_names[i], plugin->name);
        copy_string_to_char(plugin_name_library, plugin->lib_name);

        // first steps: load the common data
        if(!loadCommonPlugin(signing_authority, plugin, comp, comp_name, conf))
        {
            log_error("Unable to load common plugin data for " << plugin_names[i] << ". Giving up hope for this plugin.");
            continue;
        }

        // now load the component specific methods
        if(comp == PLUGIN_CLIENT)
        {
            // load the execute method
            P_CLIENT_EXECUTE execmth = get_method<P_CLIENT_EXECUTE>("execute", lib_handle);
            if (execmth == 0)
            {
                discard(plugin, "no execute() method for a client plugin");
                continue;
            }
            static_cast<client_plugin_descriptor*>(plugin)->f_execute = execmth;

            // load the setup method
            P_CLIENT_SETUP setupmth = get_method<P_CLIENT_SETUP>("setup", lib_handle);
            if (setupmth == 0)
            {
                discard(plugin, "no setup() method for a client plugin");
                continue;
            }
            static_cast<client_plugin_descriptor*>(plugin)->f_setup = setupmth;
        }

        // we are here, this plugin seems to be a happy one regarding signage

        //check the load() method

        // and finally, this is an accepted plugin
        log_info("plugin " <<plugin->name << " loaded from " << plugin->lib_name);
        plugins.push_back(plugin);

    }
    return plugins;
}

std::string PluginHelper::executeClientPlugin(plugin_descriptor* pd)
{
    // check if this is an internal signed plugin or not
    ALLOCATION_BEHAVIOR what_to_do = DO_NOT_FREE_ME;
    client_plugin_descriptor* cpd = static_cast<client_plugin_descriptor*>(pd);

    unsigned int ret_len = 0;
    char* c = cpd->f_execute(&what_to_do, &ret_len);
    if(ret_len == 0)
    {
        ret_len = strlen(c);
    }

    std::string s = base64_encode((const unsigned char *)c, ret_len);

    if(what_to_do == FREE_ME)
    {
        free(c);
    }
    else
    if(what_to_do == DELETE_ME)
    {
        delete [] c;
    }

    return s;
}

}

