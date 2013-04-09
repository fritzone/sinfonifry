#include "tinyxml/tinyxml.h"

#include <string>
#include <vector>

/**
* @brief The Configuration class handles the configuration of the application
* with a standard format XML files.
*
* For more details about the configuration file see the documentation.
*/
class Configuration
{
public:

    /**
     * @brief Configuration constructor
     * @param file - the configuration file
     */
    Configuration(std::string file);

    /**
     * @brief Returns the value of the given setting for the specified component
     * @param component - the component for which we require the setting
     * @param setting - the specified setting
     */
    std::string getConfigSetting(const std::string& component,
                                 const std::string& setting,
                                 const std::string& defaultValue) const;

    /**
     * @brief Returns if the configuration was loaded or not.
     * @return the status of the loadedness of the configuration.
     */
    bool loaded() const
    {
        return m_configLoaded;
    }

    /**
     * @brief getPlugins returns the names of the plugins that belong to the
     *        given component.
     * @param component The component whose plugins we want to retrieve
     * @return A vector with the names of the plugins of the component.
     */
    std::vector<std::string> getPlugins(const std::string& component) const;

    /**
     * @brief getConfigSettingForPlugin returns a specific setting for the
     * plugin with the given name of the specific component.
     * @param component - the name of the component
     * @param plugin - the name of the plugin
     * @param setting - the setting we want
     */
    std::string getConfigSettingForPlugin(const std::string& component,
                                          const std::string& plugin,
                                          const std::string& setting,
                                          std::string defaultValue) const;

private:
    TiXmlDocument m_doc;
    bool m_configLoaded;
};
