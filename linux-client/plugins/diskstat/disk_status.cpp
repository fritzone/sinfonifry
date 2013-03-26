#define __STDC_FORMAT_MACROS

#include <stdlib.h>
#include <blkid/blkid.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <ext2fs/ext2fs.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <ctime>

#include "utils.h"
#include <plugin.h>

#include <cxxtools/log.h>

log_define("sinfonifry.client.linux.plugin.disk_stat")

static const int port = 29888;

static void pretty_print_line(const char *device, const char *fs_type,
                              const char *label, const char *mtpt,
                              const char *uuid, std::stringstream& ss)
{
  unsigned long long total = 0;
  const char* target = mtpt;
  if(mtpt[0] == '(') return;
  total = get_disk_total(target);
  unsigned long long free_space = get_disk_free(target);
  ss << "<device name = \"" << device
     << "\" fs_type = \"" << fs_type
     << "\" label = \"" << label
     << "\" mountpt = \"" << mtpt
     << "\" total_bytes = \"" << total
     << "\" free_bytes = \"" << free_space << "\" />";
}

static void pretty_print_dev(blkid_dev dev, std::stringstream& ss)
{
  blkid_tag_iterate       iter;
  const char              *type, *value, *devname;
  const char              *uuid = "", *fs_type = "", *label = "";
  int                     len, mount_flags;
  char                    mtpt[1024];
  errcode_t               retval;

  if (dev == NULL) {
    return;
  }

  devname = blkid_dev_devname(dev);
  if (access(devname, F_OK))
    return;

  /* Get the uuid, label, type */
  iter = blkid_tag_iterate_begin(dev);
  while (blkid_tag_next(iter, &type, &value) == 0) {

    if (!strcmp(type, "UUID"))
      uuid = value;
    if (!strcmp(type, "TYPE"))
      fs_type = value;
    if (!strcmp(type, "LABEL"))
      label = value;
  }
  blkid_tag_iterate_end(iter);

  /* Get the mount point */
  mtpt[0] = 0;
  retval = ext2fs_check_mount_point(devname, &mount_flags,
				    mtpt, sizeof(mtpt));
  if (retval == 0) {
    if (mount_flags & EXT2_MF_MOUNTED) {
      if (!mtpt[0])
	strcpy(mtpt, "(unknown)");
    } else if (mount_flags & EXT2_MF_BUSY)
      strcpy(mtpt, "(in use)");
    else
      strcpy(mtpt, "(not mounted)");
  }

  pretty_print_line(devname, fs_type, label, mtpt, uuid, ss);
}

std::string gather_disk_stat()
{
    blkid_dev_iterate       iter;
    blkid_dev               dev;
    blkid_cache             cache = NULL;
    char *search_type = NULL, *search_value = NULL;
    char *read = NULL;

    blkid_get_cache(&cache, read);

    blkid_probe_all(cache);

    std::stringstream ss;
    ss.precision(100); // weird, but seems to work

    ss << "<devices timestamp=\"";
    std::time_t t = std::time(0);
    ss << t << "\">";
    iter = blkid_dev_iterate_begin(cache);
    blkid_dev_set_search(iter, search_type, search_value);
    while (blkid_dev_next(iter, &dev) == 0)
    {
      dev = blkid_verify(cache, dev);
      if (!dev)
        continue;
      pretty_print_dev(dev, ss);
    }
    blkid_dev_iterate_end(iter);
    ss << "</devices>";
    std::string s = ss.str();
    return s;
}


// called when the plugin container needs data. Caller needs to free the data
char* execute(ALLOCATION_BEHAVIOR *free_returned_value)
{
    std::string s = gather_disk_stat();
    char* c = (char*)calloc(s.length() + 1, sizeof(char));
    strncpy(c, s.c_str(), s.length());
    *free_returned_value = FREE_ME;
    return c;
}

// called upon loading the plugin. Internal initialization can be done
int load()
{
}

// called when the system goes down.
void unload()
{
}

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
int setup(const char*)
{
    return 1;
}
