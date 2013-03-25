#ifndef PLUGIN_HELPER_H
#define PLUGIN_HELPER_H

#include <vector>
#include "plugin.h"

class Configuration;

std::vector<plugin_descriptor*> get_plugins(PLUGIN_COMPONENT,
                                            const Configuration &conf);

#endif // PLUGIN_HELPER_H
