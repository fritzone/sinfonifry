
#include <exception>
#include <iostream>
#include <cxxtools/net/tcpstream.h>
#include <cxxtools/arg.h>
#include <cxxtools/loginit.h>
#include <cxxtools/net/tcpserver.h>
#include <cxxtools/function.h>
#include <cxxtools/thread.h>
#include <cxxtools/mutex.h>
#include <cxxtools/condition.h>
#include "tinyxml/tinyxml.h"
#include "tntdb.h"

class ClientThread : public cxxtools::DetachedThread
{
public:
  ClientThread(cxxtools::net::iostream* worker) : m_worker(worker) {}

protected:
  void run()
  {

    std::ostringstream ss;
    ss << m_worker->rdbuf();
    std::string s =ss.str();

    std::string c =m_worker->getPeerAddr();

    std::cout << c << std::endl;
    std::cout << s;

    bool host_need_initialization = true;
    uint32_t host_id = 0;

    // now do the database work
    try
    {
      tntdb::Connection conn = tntdb::connect("postgresql:dbname=sinfonifry user=sinfonifry password=s1nf0n1fry");
      //see if this host is allowed to send data
      {
      std::string query_for_find_host_in_allowed_tables = "select count(*) from sinf01_allowed_hosts where allowed_host_ip = '" + c + "'";
      tntdb::Result result = conn.select(query_for_find_host_in_allowed_tables);
      for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
      {
          tntdb::Row row = *it;
          int a = -1;
          row[0].get(a);  // read column 0 into variable a
          if(a == 0 || a == -1)
          {
              // this host is not enabled
              std::cout << "Not allowed:" << c << std::endl;
              return;
          }
      }
      }

      // see if this host is in the stage of "init"
      {
      std::string query_for_find_host_in_init_table = "select count(*) from sinf02_host_init where host_init_ip = '" + c + "'";
      tntdb::Result result = conn.select(query_for_find_host_in_init_table);
      for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
      {
          tntdb::Row row = *it;
          int a = -1;
          row[0].get(a);  // read column 0 into variable
          if(a == 0 || a == -1)
          {
              host_need_initialization = false;
          }
      }
      }

      // and insert data in the all time hosts
      std::string query_for_find_host = "select count(*) from sinf01_host where host_ip = '" + c + "'";
      tntdb::Result result = conn.select(query_for_find_host);
      for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
      {
          tntdb::Row row = *it;
          int a = -1;
          row[0].get(a);  // read column 0 into variable a
          if(a == 0)      //  this means, the host was not in the database till now
          {
              // insert the IP to the database if it is not there already
              std::string query_for_insert_host = "insert into sinf01_host(host_ip, host_name, host_last_seen_time) values('" + c + "', 'unnamed', now())";
              conn.execute(query_for_insert_host);
          }
          else
          {
              // update the host_last_seen_time for the found host
          }

          // now fetch the host_id from the host_table
          std::string query_for_find_host_id = "select host_id from sinf01_host where host_ip = '" + c + "'";
          tntdb::Result result = conn.select(query_for_find_host_id);
          for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
          {
              tntdb::Row row = *it;
              row[0].get(host_id);
          }
      }

    TiXmlDocument doc;
    bool success = doc.FromMemory(s.c_str());
    if(success)
    {
        TiXmlElement* el = doc.FirstChildElement("devices");
        if(el)
        {
            el = el->FirstChildElement("device");
            while(el)
            {
                std::string disk_part_name;
                std::string disk_mountpoint;
                uint64_t total_disk_space = 0;
                uint64_t free_disk_space = 0;
                std::string fs_type;
                std::string disk_label;

                const char* attr = el->Attribute("name");
                if(attr) { disk_part_name = attr; }

                attr = el->Attribute("mountpt");
                if(attr) { disk_mountpoint = attr; }

                attr = el->Attribute("fs_type");
                if(attr) { fs_type = attr; }

                attr = el->Attribute("total_bytes");
                if(attr) { total_disk_space = atoll(attr); }

                attr = el->Attribute("free_bytes");
                if(attr) { free_disk_space = atoll(attr); }

                attr = el->Attribute("label");
                if(attr) { disk_label = attr; }

                // create an insert statement into the disk table with the data
                // above depending on the host_need_initialization, use the host_id
                if(host_need_initialization)
                {
                    std::stringstream ss;
                    ss << "insert into sinf01_disk (host_id, disk_fs_type, disk_mountpoint, disk_physical_id, disk_label, disk_total_space) values (";
                    ss << host_id << ", '" << fs_type << "', '" << disk_mountpoint << "', '" << disk_part_name << "', '" << disk_label << "', " << total_disk_space << ")";
                    std::string query_for_insert_disk = ss.str();
                    conn.execute(query_for_insert_disk);

                    // and now delete the host from the init table
                    std::string query_for_remove_host_from_init_table = "delete from sinf02_host_init where host_init_ip = '" + c + "'";
                    conn.execute(query_for_remove_host_from_init_table);
                }

                // and here finally insert the data into the statistics table

                el = el->NextSiblingElement("device");
            }
        }
        else
        {
            std::cout << "no root" << std::endl;
        }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

private:
  cxxtools::net::iostream* m_worker;
};

static const int BUFSIZE = 1024 * 1024;

int main(int argc, char* argv[])
{
  try
  {
    log_init();
    cxxtools::Arg<const char*> ip(argc, argv, 'i', "0.0.0.0");
    // listen to a port
    cxxtools::net::TcpServer server(ip.getValue(), 29888);

    while(1)
    {
      // accept a connetion
      cxxtools::net::iostream* worker = new cxxtools::net::iostream(server, BUFSIZE);
      (new ClientThread(worker))->start();

    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

