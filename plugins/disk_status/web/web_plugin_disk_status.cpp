#include "sinfonifry_plugin_base.h"
#include "sinfonifry_plugin_vis.h"
#include <configuration.h>
#include <tntdb.h>
#include "db_utils.h"
#include <algorithm>

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

char* data_request(ALLOCATION_BEHAVIOR *free_returned_value, const char *ip, const char *div_name)
{
    // the host ID
    uint32_t host_id = 0;
    Configuration conf = Configuration::defaultConfiguration();
    tntdb::Connection conn = tntdb::connect(sinfonifry::get_master_connection_string(conf));

    std::string query_for_find_host_id = std::string("select host_id from sinf01_host where host_ip = '") + ip + "'";
    tntdb::Result result = conn.select(query_for_find_host_id);
    for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
    {
        tntdb::Row row = *it;
        row[0].get(host_id);
    }
    std::string b = ip;
    std::replace(b.begin(), b.end(), '.', '_');

    std::stringstream reply;

    std::stringstream ss;
    ss << "select distinct b.disk_mountpoint, b.disk_id, a.disk_stat_free_space, a.disk_stat_measurement_time, b.disk_total_space, b.disk_physical_id "
       << "from sinf03_disk b, sinf03_disk_statistics a "
       << "where b.host_id = " << host_id << " and a.disk_stat_disk_id =b.disk_id and "
       << "a.disk_stat_measurement_time = (select max(disk_stat_measurement_time) "
       << "from sinf03_disk_statistics c, sinf03_disk d "
       << "where c.disk_stat_disk_id=d.disk_id and d.host_id="<< host_id <<") "
       << "group by b.disk_mountpoint, b.disk_id, a.disk_stat_free_space, a.disk_stat_measurement_time "
       << "order by a.disk_stat_measurement_time desc";
    tntdb::Result diskDataResult = conn.select(ss.str());
    reply << "    <table border=\"0\" cellspacing=\"0\" id=\"tableDiskDataFor" << b << "\">";
    // the header of the table
    reply << "     <tr class=\"d0\">";
      reply << "      <td width=\"300px\">";
      reply << "<p class=\"table_header\">Mountpoint</p>";
      reply << "      </td>";
      reply << "      <td width=\"100px\">";
      reply << "<p class=\"table_header\">Free</p>";
      reply << "      </td>";
      reply << "      <td width=\"100px\">";
      reply << "<p class=\"table_header\">Total</p>";
      reply << "      </td>";
      reply << "      <td width=\"100px\">";
      reply << "<p class=\"table_header\"><b>%Free</b></p>";
      reply << "      </td>";
      reply << "      <td width=\"100px\">";
      reply << "<p class=\"table_header\"><b>Status</b></p>";
      reply << "      </td>";
      reply << "     </tr>";
    // and here generate the disk data for the current host based on data from the DB
    for (tntdb::Result::const_iterator it2 = diskDataResult.begin(); it2 != diskDataResult.end(); ++it2)
    {
      tntdb::Row row = *it2;
      std::string mountpt;
      uint64_t freeSpace;
      uint64_t totalSpace;
      uint64_t disk_id;
      std::string physId;
      row[0].get(mountpt);
      row[1].get(disk_id);
      row[2].get(freeSpace);
      row[4].get(totalSpace);
      row[5].get(physId);
      if(totalSpace > 0) // do not go in there if there is no space on the device (swap for example)
      {
        reply << "     <tr>";
        // the mountpoint
        reply << "      <td>";
        reply << "<p class=\"table_text\" title=\"" << physId << "\">"     ;
        // TODO: This was a sout() !!!
        reply << "<@ep name=\"disk_details\" params=\"id=" << disk_id << "\" display=\"" << mountpt << "\">";
        reply <<"</p>";
        reply << "      </td>";
        // the free space
        reply << "      <td>";
        if(freeSpace < 1073741824) // under 1 GB we show MB
          reply << "<p class=\"table_text\">" << freeSpace  / 1048576<< "<b>M</b>b</p>";
        else
          reply << "<p class=\"table_text\" title=\"" <<freeSpace << " bytes\">" << freeSpace  / 1073741824<< "<b>G</b>b</p>";
        reply << "      </td>";
        // the total space
        reply << "      <td>";
        if(totalSpace < 1073741824)
          reply << "<p class=\"table_text\">" << totalSpace / 1048576 << "Mb</p>";
        else
          reply << "<p class=\"table_text\">" << totalSpace / 1073741824 << "Gb</p>";
        reply << "      </td>";
        reply << "      <td>";
        reply << "<p class=\"table_text\">";
        // TODO: This was a sout below
        reply << 100 * freeSpace / totalSpace;
        reply <<"%</p>";
        reply << "      </td>";

        std::string iconToUse = "ok.png";
        if(100 * freeSpace / totalSpace < 5)
        { // }
        iconToUse = "error.png";
        }
        else
        if(100 * freeSpace / totalSpace < 15)
        {// }
        iconToUse = "warning.png";
        }
        reply << "      <td>";
        reply << "       <img src=\"" << iconToUse <<"\" width=\"22\" height=\"22\"/>";
        reply << "      </td>";

        reply << "     </tr>\n";
      } // if totalSpace
    }// the FOR
    reply << "    </table>"; // the disk data table closure of tableDiskDataFor
    char *s = new char [reply.str().length() + 1] ;
    strcpy(s, reply.str().c_str());
    s[reply.str().length()] = 0;
    *free_returned_value = DELETE_ME;
    return s;
}

const char* styles()
{
    static const char* s = "\
            p.table_header {\
              font-family: sans-serif;\
              font-style: normal;\
              font-variant: normal;\
              font-weight: bold;\
              font-size: medium;\
              line-height: 100%;\
              word-spacing: normal;\
              letter-spacing: normal;\
              text-decoration: none;\
              text-transform: none;\
              text-align: left;\
              text-indent: 0ex;\
            }\
            p.table_text {\
              font-family: sans-serif;\
              font-style: normal;\
              font-variant: normal;\
              font-weight: normal;\
              font-size: medium;\
              line-height: 100%;\
              word-spacing: normal;\
              letter-spacing: normal;\
              text-decoration: none;\
              text-transform: none;\
              text-align: left;\
              text-indent: 0ex;\
            }\
            tr.d0 {\
                    background-color: #C0C9D9; color: black;\
            }\
            ";

    return s;
}

const char* const* entrypoints()
{
    static const char* arrays[] = {"test123", "test2", 0};
    return arrays;
}

const char* javascripts()
{
    return 0;
}
