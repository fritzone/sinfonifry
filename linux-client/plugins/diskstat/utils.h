#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>

/**
 * @brief get_disk_free returns the free disk space available on the given path
 */
uint64_t get_disk_free(const char* path);

/**
 * @brief get_disk_total returns the total disk space of the given path
 */
uint64_t get_disk_total(const char* path);

#endif // UTILS_H
