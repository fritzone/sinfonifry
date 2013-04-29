#include "sinfonifry_plugin_base.h"
#include "sinfonifry_plugin_vis.h"

// an internal about string. User should NOT free the data returned from here
const char* about()
{
    return "disk_status plugin 0.1 for sinfonifry. (c) 2012 The UnaFrog project";
}

// the name of the plugin. User should NOT free this
const char* name()
{
    return "disk_status";
}

// does some setup work. Plugin dependent
const char* setup(const char*)
{
    return 0;
}

PLUGIN_COMPONENT component()
{
    return PLUGIN_VISUALIZATION;
}

const char* signature()
{
    return "disk_status_una_frog";
}

const char* descriptive_name()
{
    return "Disk Status";
}
