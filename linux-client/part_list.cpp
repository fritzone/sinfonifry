#include <stdlib.h>
#include <blkid/blkid.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <ext2fs/ext2fs.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cxxtools/net/tcpstream.h>
#include <cxxtools/arg.h>

#include "tinyxml/tinyxml.h"

static const int port = 29888;

static double get_disk_free(const char* path)
{
    struct stat stst;
    struct statfs stfs;

    if ( stat(path, &stst) == -1 )
    {
        return 0.0;
    }

    if ( statfs(path, &stfs) == -1 )
    {
        return 0.0;
    }

    return stfs.f_bavail * ( stst.st_blksize);

}

static double get_disk_total(const char* path)
{
    struct stat stst;
    struct statfs stfs;

    if ( stat(path, &stst) == -1 )
    {
        return 0.0;
    }

    if ( statfs(path, &stfs) == -1 )
    {
        return 0.0;
    }
    return stfs.f_blocks * ( stst.st_blksize);
}


static void pretty_print_line(const char *device, const char *fs_type,
                              const char *label, const char *mtpt,
                              const char *uuid, std::stringstream& ss)
{
  double total = 0.0;
  const char* target = mtpt;
  if(mtpt[0] == '(') return;
  total = get_disk_total(target);
  double free_space = get_disk_free(target);
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

    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><devices>";

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

int main(int argc, char* argv[])
{
    TiXmlDocument doc;
    doc.LoadFile("/etc/sinfonifry/client-config.xml");
    if(doc.Error())
    {
        std::cerr << "Cannot create XML document from config file or config file is not there." << std::endl;
        return 1;
    }

    std::string core_host = "127.0.0.1";
    int port = 29888;

    TiXmlElement* el = doc.FirstChildElement("sinfonifry");
    if(el)
    {
        el = el->FirstChildElement("client");
        if(!el)
        {
            std::cerr << "Wrong xml file. No <client> node in <sinfonifry> node" << std::endl;
            return 1;
        }
        el = el->FirstChildElement("config");
        if(!el)
        {
            std::cerr << "Wrong xml file. No <config> node in <client> node" << std::endl;
            return 1;
        }
        // get the core host IP
        const char* att_core_host = el->Attribute("core-host");
        if(!att_core_host)
        {
            std::cerr << "Wrong xml file. No core-host attribute in <config> node" << std::endl;
            return 1;
        }
        core_host = att_core_host;

        // get the core host port
        const char* att_core_port = el->Attribute("core-port");
        if(!att_core_port)
        {
            std::cerr << "Wrong xml file. No core-port attribute in <config> node, using default 29888" << std::endl;
        }
        else
        {
            port = atoi(att_core_port);
            if(port == 0)
            {
                std::cerr << "Wrong xml file. core-port attribute in <config> node is wrong, using default 29888" << std::endl;
                port = 29888;
            }
        }

    }
    else
    {
        std::cerr << "Wrong xml file. No <sinfonifry> node" << std::endl;
        return 1;
    }
    int exception_counter = 1;
    while(1)
    {
        std::string s = gather_disk_stat();

        try
        {

        cxxtools::Arg<const char*> ip(argc, argv, 'i', core_host.c_str());

        cxxtools::net::iostream conn(ip.getValue(), port);
        conn.write(s.c_str(), s.length());
        conn.flush();
        conn.close();
        exception_counter = 1;
        }
        catch(const std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            exception_counter ++;
        }
        std::cout << "sleeping: " << exception_counter << std::endl;
        sleep(exception_counter);
    }
}
