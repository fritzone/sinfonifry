#include "tinyxml/tinyxml.h"

#include <string>
#include <vector>

/**
* @brief The Configuration class handles the configuration of the application
* with a standard format XML files.
* The following is the XML configuration standard:
* <sinfonifry>
* <client/server (as component)>
* <config SETTING1="value" SETTING2="value" />
* <plugins>
* <plugin name="NAME" enabled="1/0" />
* </plugins>
* </client/server>
* </sinfonifry>
*
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
                                 std::string defaultValue);

    bool loaded() const
    {
        return m_configLoaded;
    }

    /**
* @brief getPlugins return the plugins that
* @param component
* @return
*/
    std::vector<std::string> getPlugins(const std::string& component) const;

    /**
* @brief getConfigSettingForPlugin Return a specific setting for the
* plugin with the given name of the specific component.
* @param component - the name of the component
* @param plugin - the name of the plugin
* @param setting - the setting we want
*/
    std::string getConfigSettingForPlugin(const std::string& component,
                                          const std::string& plugin,
                                          const std::string& setting,
                                          std::string defaultValue);

private:
    TiXmlDocument m_doc;
    bool m_configLoaded;
};
