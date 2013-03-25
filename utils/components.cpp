#include "components.h"

std::string component_name_from_component_code(PLUGIN_COMPONENT code)
{
    if(code == PLUGIN_CLIENT) return std::string("client");

    return "unknown";
}
