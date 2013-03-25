#include "utils.h"

#include <sys/vfs.h>
#include <sys/statvfs.h>

/**
 * @brief get_disk_free returns the free disk space
 */
uint64_t get_disk_free(const char* path)
{
    struct statvfs stfs;

    if ( statvfs(path, &stfs) == -1 )
    {
        return 0.0;
    }
    uint64_t t =  (uint64_t)stfs.f_bsize *  (uint64_t)stfs.f_bfree ;
    return t;

}

/**
 * @brief get_disk_total
 */
uint64_t get_disk_total(const char* path)
{
    struct statvfs stfs;
    if ( statvfs(path, &stfs) == -1 )
    {
        return 0;
    }
    uint64_t t = (uint64_t)stfs.f_blocks * (uint64_t)stfs.f_bsize;
    return t;
}

