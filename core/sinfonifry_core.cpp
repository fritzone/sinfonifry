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
#include "configuration.h"
#include <instdir.h>
#include <plugin_helper.h>
#include <db_utils.h>
#include <b64.h>

log_define("sinfonifry.core")

using namespace sinfonifry;

class ClientThread : public cxxtools::DetachedThread
{
public:
  ClientThread(cxxtools::net::iostream* worker, const Configuration* conf) :
    m_worker(worker), m_conf(conf) 
  {}
  
private:
  
  /* return true if the host which just connected to us is allowed in the DB to connect */
  bool hostIsAllowed(const std::string& c)
  {
      try
      {
          tntdb::Connection conn = tntdb::connect(sinfonifry::get_master_connection_string(m_conf));

          std::string query_for_find_host_in_allowed_tables = "select count(*) from sinf01_allowed_hosts where allowed_host_ip = '" + c + "'";
          tntdb::Result result = conn.select(query_for_find_host_in_allowed_tables);
          tntdb::Row row = *result.begin();
          int a = -1;
          row[0].get(a);  // read column 0 into variable a
          if(a == 0 || a == -1)
          {
              // this host is not enabled
              log_error("Host is not allowed to connect: " << c);
              m_worker->close();
              return false;
          }

          conn.close();
      }
      catch (const std::exception& e)
      {
              log_error("Cannot check if the host is allowed to connect or not: " << e.what());
              m_worker->close();
              return false;
      }
      
      return true;
  }

protected:
  void run()
  {

    std::ostringstream ss;
    ss << m_worker->rdbuf();
    std::string s =ss.str();
    log_debug(s);
    std::cout << s << std::endl;
    std::string c = m_worker->getPeerAddr();

    // will be switched to true if the host needs initialization
    bool host_need_initialization = true;
    
    // the host ID
    uint32_t host_id = 0;
    
    // first step: see if this client is allowed to connect to us
    if(!hostIsAllowed(c))
    {
        return;
    }

    try
    {
        tntdb::Connection conn = tntdb::connect(sinfonifry::get_master_connection_string(m_conf));

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

        bool update_host_last_seen_time = false;

        // and insert data in the all time hosts
        {
            //firstly get the host name from the host_init table
            std::string host_name = "";
            {
                std::string query_for_find_host = "select allowed_host_name from sinf01_allowed_hosts where allowed_host_ip = '" + c + "'";
                tntdb::Result result = conn.select(query_for_find_host);
                tntdb::Row row = *result.begin();
                row[0].get(host_name);  // read column 0 into variable a
            }

            std::string query_for_find_host = "select count(*) from sinf01_host where host_ip = '" + c + "'";
            tntdb::Result result = conn.select(query_for_find_host);
            tntdb::Row row = *result.begin();
            int a = -1;
            row[0].get(a);  // read column 0 into variable a
            if(a == 0)      //  this means, the host was not in the database till now
            {
                // insert the IP to the database if it is not there already
                std::string query_for_insert_host = "insert into sinf01_host(host_ip, host_name, host_last_seen_time) values('" + c + "', '" + host_name + "', now())";
                conn.execute(query_for_insert_host);
            }
            else
            {
                update_host_last_seen_time = true;
            }
        }

        // now fetch the host_id from the host_table
        {
            std::string query_for_find_host_id = "select host_id from sinf01_host where host_ip = '" + c + "'";
            tntdb::Result result = conn.select(query_for_find_host_id);
            for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
            {
                tntdb::Row row = *it;
                row[0].get(host_id);
            }
        }

        // update the host last seen time?
        if(update_host_last_seen_time)
        {
            std::stringstream ss;
            ss << "update sinf01_host set host_last_seen_time=now() where host_id = " << host_id;
            conn.execute(ss.str());
        }

        TiXmlDocument doc;
        bool success = doc.FromMemory(s.c_str());
        if(success)
        {
          
            // find the plugin_data node
            TiXmlElement* el_for_plugin_data = doc.FirstChildElement("plugin_data");

            if(el_for_plugin_data)
            {
                // find all the "plugin_name" child nodes
                TiXmlElement* el_for_plugin_name= el_for_plugin_data->FirstChildElement("plugin");
                if(!el_for_plugin_name)
                {
                    log_warn("No \"plugin\" node in the plugin_data");
                    return;
                }

                while(el_for_plugin_name)
                {
                    const char* plugin_name = el_for_plugin_name->Attribute("name");
                    if(!plugin_name)
                    {
                        log_warn("malformatted XML from client. No plugin name");
                        return;
                    }

                    // now loop through the plugin entries
                    std::string plugin_data;
                    el_for_plugin_name->FirstChild()->Print(plugin_data, 1);

                    std::string decoded = base64_decode(plugin_data);

                    // now see if the host needs an initialization phase from the plugins
                    if(host_need_initialization)
                    {

                    }

                    el_for_plugin_name = el_for_plugin_name->NextSiblingElement("plugin");
                }

                return;
            }
            else
            {
                log_error("No plugin_data node");
                return;
            }

            conn.close();
        }
    }
    catch (const std::exception& e)
    {
        log_error(e.what());
    }
    m_worker->close();
  }

private:
  cxxtools::net::iostream* m_worker;
  const Configuration* m_conf;
};

static const int BUFSIZE = 1024 * 1024;

int main(int argc, char* argv[])
{
    log_init("log.properties");

    try
    {
        std::string inst(instdir);
        Configuration *conf = new Configuration(inst + "/sinfonifry/config/config.xml");
        if(!conf->loaded())
        {
            log_error("Exiting due to lack of configuration");
            return 1;
        }

        std::vector<plugin_descriptor*> plugins = PluginHelper::instance().getSignedPlugins(PLUGIN_CORE, conf, 0);

        std::string ip = conf->getConfigSetting("core", "bind_to", "0.0.0.0");
        int port = atoi(conf->getConfigSetting("core", "bind_port", "29888").c_str());

        // listen to a port
        cxxtools::net::TcpServer server(ip.c_str(), port);

        while(1)
        {
            // accept a connetion
            cxxtools::net::iostream* worker = new cxxtools::net::iostream(server, BUFSIZE);
            (new ClientThread(worker, conf))->start();

        }
    }
    catch (const std::exception& e)
    {
        log_error(e.what());
    }
}

