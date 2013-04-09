#ifndef PLUGIN_HELPER_H
#define PLUGIN_HELPER_H

#include <vector>
#include <map>

#include "sinfonifry_plugin_client.h"
#include "sinfonifry_plugin_base.h"
#include "sinfonifry_signed_plugin.h"
#include "components.h"

class Configuration;

namespace sinfonifry
{

/*
 * Data below is used by the components to identify the plugin structure. This
 * class is used only if the plugin is signed. Unsigned plugins are executed via
 * a wrapper.
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

    // the pointer to the component() method
    P_COMPONENT f_component;

    // the pointer to the about() method
    P_ABOUT f_about;

    // the pointer to the name() method
    P_NAME f_name;

    // the pointer to the signature() method
    P_SIGNATURE f_signature;

    // the pointer to the last_erro() method
    P_LAST_ERROR f_last_error;
};

template <class T> T get_method(const char* mth, void* lib_handle);

struct signed_plugin : public plugin_descriptor
{
    P_LOAD f_load;

    P_UNLOAD f_unload;
};

struct client_plugin_descriptor : public plugin_descriptor
{
    P_CLIENT_EXECUTE f_execute;

    P_CLIENT_SETUP f_setup;
};

/**
 * @brief Simple class for managing the plugins of a running instance
 */
class PluginHelper
{

public:

    static PluginHelper& instance();

    /**
     * @brief Loads and returns the signed plugins that can be loaded
     *        for the given component.
     * @param conf
     * @param comp
     * @return
     */
    std::vector<plugin_descriptor*> getSignedPlugins(PLUGIN_COMPONENT comp,
                                                const Configuration *conf, const char *signing_authority);

    bool loadCommonPlugin(const char* signing_authority, plugin_descriptor *&plugin, PLUGIN_COMPONENT comp, std::string comp_name, const Configuration* conf);
private:

    PluginHelper();

    /**
     * @brief Add a forbidden plugin to this session.
     *
     * Forbidden plugins will not be loaded in this session anymore since the contain
     * malformed plugin libraries (ie: missing names, etc...)
     *
     * @param name
     * @param reason
     */
    void addForbiddenPlugin(const std::string& name, const std::string& reason);

    /**
     * @brief Check the signature that is passed in, return true if it is valid
     */
    bool checkSignature(const char*, const char *);

    /**
     * @brief discards the plugin, logs the error.
     */
    void discard(plugin_descriptor *&, const char*);


private:

    std::map<std::string, std::string> m_forbiddenPlugins;

    std::vector<plugin_descriptor*> m_signedPlugins;
};

} // namespace

#endif // PLUGIN_HELPER_H
