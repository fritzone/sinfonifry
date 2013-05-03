#ifndef UTILS_WEB_H
#define UTILS_WEB_H

#include <string>

namespace sinfonifry
{

/**
 * @brief fix_entrypoints fixes the entrypoints and generates valid html
 * @param s
 * @return
 */
std::string fix_entrypoints(const std::string& s, const std::string& plugin);

}

#endif // UTILS_WEB_H
