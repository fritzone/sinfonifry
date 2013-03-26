#include "configuration.h"

#include <iostream>

using namespace std;

Configuration::Configuration(std::string file) : m_doc(), m_configLoaded(false)
{
    m_doc.LoadFile(file.c_str());
    if(m_doc.Error())
    {
        std::cerr << "Cannot load config file:" << file << std::endl;
        return;
    }
    m_configLoaded = true;
}

std::string Configuration::getConfigSetting(const std::string& component,
                                            const std::string& setting,
                                            std::string defaultValue)
{
    if(!m_configLoaded) return defaultValue;

    TiXmlElement* el = m_doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement(component.c_str());
        if(!el)
        {
            std::cerr << "Wrong xml file. No component node" << std::endl;
            return defaultValue;
        }
        el = el->FirstChildElement("configuration");
        if(!el)
        {
            std::cerr << "Wrong xml file. No <configuration> in component node"
                      << std::endl;
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
            std::cerr << "Wrong xml file. No component node" << std::endl;
            return result;
        }
        el = el->FirstChildElement("plugins");
        if(!el)
        {
            std::cerr << "Wrong xml file. No <plugins> node in component node"
                      << std::endl;
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
                                      std::string defaultValue)
{
    if(!m_configLoaded) return defaultValue;

    const TiXmlElement* el = m_doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement(component.c_str());
        if(!el)
        {
            std::cerr << "Wrong xml file. No component node" << std::endl;
            return defaultValue;
        }
        el = el->FirstChildElement("plugins");
        if(!el)
        {
            std::cerr << "Wrong xml file. No <plugins> node in component node"
                      << std::endl;
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
                    const char* attr = el_plugin->Attribute(setting.c_str());
                    if(!attr)
                    {
                        std::cerr << "No " << setting << " in the config file"
                                 << " for plugin " << plugin << ", component"
                                  << component << "." << std::endl;
                        return defaultValue;
                    }
                    return std::string(attr);
                }
            }

            el_plugin = el_plugin->NextSiblingElement();
        }
    }
    return defaultValue;
}
