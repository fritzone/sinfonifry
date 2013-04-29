#include "configuration.h"
#include "ClientThread.h"
#include "plugin_helper.h"

#include <cxxtools/loginit.h>
#include <cxxtools/net/tcpserver.h>

#include <exception>
#include <iostream>

log_define("sinfonifry.core")



static const int BUFSIZE = 1024 * 1024; // the size the server can receive at once...
using namespace sinfonifry;

int main(int argc, char* argv[])
{
    log_init("log.properties");

    try
    {
        // load the configuration
        const Configuration& conf = Configuration::defaultConfiguration();

        // initialize the proper PluginHelper
        PluginHelper::initialize(PLUGIN_CORE, conf);

        // load the configuration settings
        std::string ip = conf.getConfigSetting("core", "bind_to", "0.0.0.0");
        int port = atoi(conf.getConfigSetting("core", "bind_port", "29888").c_str());

        // open a server and listen to the given port
        cxxtools::net::TcpServer server(ip.c_str(), port);

        // wait for connections
        while(1)
        {
            cxxtools::net::iostream* worker = new cxxtools::net::iostream(server, BUFSIZE);

            // and start them in a new thread
            (new ClientThread(worker, conf))->start();
        }

        return 0;
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

    return 1;
}

