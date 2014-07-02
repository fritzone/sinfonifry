#include "components.h"
#include <cxxtools/log/cxxtools.h>

log_define("sinfonifry.components");

namespace sinfonifry
{

std::string component_name_from_component_code(PLUGIN_COMPONENT code)
{
    if(code == PLUGIN_CLIENT) return std::string("client");
    if(code == PLUGIN_CORE) return std::string("core");
    if(code == PLUGIN_VISUALIZATION) return std::string("vis");

    log_error("Unknow component: " << code);
    return "unknown";
}

}

