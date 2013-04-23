#include "configuration.h"
#include <instdir.h>
#include <cxxtools/net/tcpstream.h>
#include <cxxtools/log.h>

#include <iostream>
#include <unistd.h>

#include <sinfonifry_plugin_client.h>
#include <plugin_helper.h>

using namespace sinfonifry;

log_define("sinfonifry.client.linux")

int main(int argc, char* argv[])
{
    log_init("log.properties");
    log_info("### Starting as PID " << getpid());

    try
    {
        // load the configuration
        const Configuration& conf = Configuration::defaultConfiguration();

        // initialize the proper PluginHelper
        PluginHelper::initialize(PLUGIN_CLIENT, conf);

        // load some config data
        std::string core_host = conf.getConfigSetting("client", "core-host", "127.0.0.1");
        int port = atoi(conf.getConfigSetting("client", "core-port", "29888").c_str());
        int sleep_time = atoi(conf.getConfigSetting("client", "sleep-time-sec", "60").c_str());

        std::vector<plugin_descriptor*> plugins = PluginHelper::instance().getSignedPlugins();
        int exception_counter = 1;
        while(1)
        {

            std::string main_xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                    "<plugin_data>";

            for(int i=0; i<plugins.size(); i++)
            {
                main_xml += "<plugin name=\"";
                main_xml += plugins[i]->name;
                main_xml += "\">";

                std::string encoded_plugin_result = PluginHelper::instance().executeClientPlugin(plugins[i]);

                main_xml += encoded_plugin_result;
                main_xml += "</plugin>";

            }
            main_xml += "</plugin_data>";

            std::string s = main_xml;

            try
            {
                cxxtools::net::iostream conn(core_host.c_str(), port);
                conn.write(s.c_str(), s.length());
                conn.flush();
                conn.close();
                exception_counter = 1;
            }
            catch(const std::exception& ex)
            {
                log_error(ex.what());
                exception_counter ++;
            }
            sleep(exception_counter * sleep_time);
        }
    }
    catch (const std::exception& e)
    {
        log_error(e.what());
    }
    catch(std::string& e)
    {
        log_error(e);
    }
    catch(...)
    {
        log_error("unknown exception in main()");
    }
}

