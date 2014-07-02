#include <tnt/tntnet.h>

#include <cxxtools/log/cxxtools.h>

#include <plugin_helper.h>
#include <configuration.h>

log_define("sinfonifry.web")

using namespace sinfonifry;

int main(int argc, char* argv[])
{
  try
  {
        
        log_init("log.properties");

        // load the configuration
        const Configuration& conf = Configuration::defaultConfiguration();

        // initialize the proper PluginHelper
        PluginHelper::initialize(PLUGIN_VISUALIZATION, conf);

        tnt::Tntnet app;
        app.listen("0.0.0.0", 8000);
        app.mapUrl("/sinfonifry", "sinfonifry");
        app.mapUrl("/host_list", "host_list");
        app.mapUrl("/dashboard", "dashboard");
        app.mapUrl("/menu", "menu");
        app.mapUrl("/host_info", "host_info");

        app.mapUrl("/add_host", "add_host");
        app.mapUrl("/get_data", "get_data");
        app.mapUrl("/get_host_status", "get_host_status");

        app.mapUrl("/(.*).png", "images");
        app.mapUrl("/(.*).js", "jscript");
        app.mapUrl("/(.*).css", "css");
        
        log_info("App starting");

        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
