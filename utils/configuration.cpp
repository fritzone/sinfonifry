#include "configuration.h"
#include <cxxtools/log.h>
#include <instdir.h>

#include <iostream>

log_define("sinfonifry.util.configuration")

using namespace std;

Configuration::Configuration(const std::string& file) : m_doc(), m_configLoaded(false)
{
    m_doc.LoadFile(file.c_str());
    if(m_doc.Error())
    {
        std::stringstream ss;
        ss << "Cannot load/parse config file:" << file << ". Reason: " << m_doc.ErrorDesc();
        log_error(ss.str());
        throw ss.str();
    }
    log_info("Opened configuration file:" << file);
    m_configLoaded = true;
}

Configuration& Configuration::defaultConfiguration()
{
    static std::string inst(instdir);
    inst += "/sinfonifry/config/config.xml";
    static Configuration defaultConfig(inst);
    return defaultConfig;
}

std::string Configuration::getConfigSetting(const std::string& component,
                                            const std::string& setting,
                                            const std::string& defaultValue)
const
{
    if(!m_configLoaded) return defaultValue;

    const TiXmlElement* el = m_doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement(component.c_str());
        if(!el)
        {
            log_error ("Wrong xml file. No component node: " << component);
            return defaultValue;
        }
        el = el->FirstChildElement("config");
        if(!el)
        {
            log_error ("Wrong xml file. No <config> node in component node");
            return defaultValue;
        }
        // get the attribute
        const char* attr = el->Attribute(setting.c_str());
        if(!attr)
        {
            return defaultValue;
        }
        return std::string(attr);
    }
    return defaultValue;
}

vector<string> Configuration::getPlugins(const std::string &component) const
{
    vector<string> result;
    if(!m_configLoaded) return result;

    const TiXmlElement* el = m_doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement(component.c_str());
        if(!el)
        {
            log_error ("Wrong xml file. No component node: " << component);
            return result;
        }
        el = el->FirstChildElement("plugins");
        if(!el)
        {
            log_error("Wrong xml file. No <plugins> node in component node");
            return result;
        }
        // now loop through the plugins and the ones that are enabled send back
        const TiXmlElement* el_plugin = el->FirstChildElement("plugin");
        while(el_plugin)
        {
            string plugin_name;
            string plugin_enabled;
            const char* name = el_plugin->Attribute("name");
            if(name) plugin_name = string(name);
            const char* enabled = el_plugin->Attribute("enabled");
            if(enabled) plugin_enabled = string(enabled);

            if(plugin_enabled == "1")
            {
                result.push_back(plugin_name);
            }

            el_plugin = el_plugin->NextSiblingElement();
        }
    }
    return result;
}

string Configuration::getConfigSettingForPlugin(const std::string& component,
                                      const std::string& plugin,
                                      const std::string& setting,
                                      std::string defaultValue) const
{
    if(!m_configLoaded) return defaultValue;

    const TiXmlElement* el = m_doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement(component.c_str());
        if(!el)
        {
            log_error ("Wrong xml file. No component node:" << component);
            return defaultValue;
        }
        el = el->FirstChildElement("plugins");
        if(!el)
        {
            log_error("Wrong xml file. No <plugins> node in component node");
            return defaultValue;
        }
        // now loop through the plugins and the ones that are enabled send back
        const TiXmlElement* el_plugin = el->FirstChildElement("plugin");
        while(el_plugin)
        {
            string plugin_name;
            string plugin_enabled;
            const char* name = el_plugin->Attribute("name");
            if(name) plugin_name = string(name);
            const char* enabled = el_plugin->Attribute("enabled");
            if(enabled) plugin_enabled = string(enabled);

            if(plugin_enabled == "1")
            {
                // now see if the name is the one we want
                if(plugin == plugin_name)
                {
                    const TiXmlElement* el_config = el_plugin->FirstChildElement("config");
                    if(!el_config)
                    {
                        log_warn("Misused xml file. No <config> node for the plugin node. Trying the attributes.");

                        const char* attr = el_plugin->Attribute(setting.c_str());
                        if(!attr)
                        {
                            log_error("No " << setting << " in the config file"
                                      << " for plugin " << plugin << ", component" << component << ".");
                            return defaultValue;
                        }
                        return std::string(attr);
                    }
                    else
                    {
                        const char* attr = el_config->Attribute(setting.c_str());
                        if(!attr)
                        {
                            log_error("No " << setting << " in the config file" << " for plugin "
                                      << plugin << ", component " << component << ".");
                            return defaultValue;
                        }
                        return std::string(attr);
                    }
                }
            }

            el_plugin = el_plugin->NextSiblingElement();
        }
    }
    return defaultValue;
}
