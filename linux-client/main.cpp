#include "configuration.h"
#include <instdir.h>
#include <cxxtools/net/tcpstream.h>
#include <cxxtools/log.h>

#include <iostream>
#include <unistd.h>

#include <sinfonifry_plugin_client.h>
#include <plugin_helper.h>

log_define("sinfonifry.client.linux")

int main(int argc, char* argv[])
{
    std::string inst(instdir);
    log_init();
    Configuration conf(inst + "/sinfonifry/client/config/config.xml");
    if(!conf.loaded())
    {
        log_error("Exiting due to lack of configuration");
        return 1;
    }

    std::string core_host = conf.getConfigSetting("client", "core-host",
                                                  "127.0.0.1");
    int port = atoi(conf.getConfigSetting("client",
                                          "core-port", "29888").c_str());
    int sleep_time = atoi(conf.getConfigSetting("client", "sleep-time-sec",
                                                "60").c_str());

    std::vector<plugin_descriptor*> plugins = get_plugins(PLUGIN_CLIENT, conf);
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
            ALLOCATION_BEHAVIOR what_to_do = DO_NOT_FREE_ME;
            plugin_descriptor* pd = plugins[i];
            client_plugin_descriptor* cpd =
                    static_cast<client_plugin_descriptor*>(pd);
            char* c = cpd->f_execute(&what_to_do);
            main_xml += c;
            main_xml += "</plugin>";
            if(what_to_do == FREE_ME)
            {
                free(c);
            }
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

