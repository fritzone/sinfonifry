#include "configuration.h"

#include <cxxtools/net/tcpstream.h>

#include <iostream>
#include <dlfcn.h>

int main(int argc, char* argv[])
{
    Configuration conf("/opt/sinfonifry/client/config/config.xml");
    if(!conf.loaded())
    {
        std::cerr << "Exiting due to configuration error" << std::endl;
        return 1;
    }

    std::string core_host = conf.getConfigSetting("client", "core-host",
                                                  "127.0.0.1");
    int port = atoi(conf.getConfigSetting("client",
                                          "core-port", "29888").c_str());
    int sleep_time = atoi(conf.getConfigSetting("client", "sleep-time-sec",
                                                "60").c_str());

    std::vector<std::string> plugins = conf.getPlugins("client");
    std::vector<void*> plugin_handles;
    for(int i=0; i<plugins.size(); i++)
    {
        std::string plugin_name = "libsinfonifry_plugin_" + plugins[i];
        plugin_name += ".so";
        plugin_name = "/opt/sinfonifry/client/plugins/" + plugin_name;
        void *lib_handle = dlopen(plugin_name.c_str(), RTLD_LAZY);
        if(!lib_handle)
        {
            std::cerr << "Cannot load plugin: "<<plugin_name <<std::endl;
            return 1;
        }
    }

    int exception_counter = 1;
    while(1)
    {
        std::string s = "";//gather_disk_stat();

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
            std::cout << ex.what() << std::endl;
            exception_counter ++;
        }
        std::cout << "sleeping: " << exception_counter * sleep_time << std::endl;
        sleep(exception_counter * sleep_time);
    }
}

