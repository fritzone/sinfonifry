#include "client_thread.h"

ClientThread::ClientThread(cxxtools::net::iostream* worker) : m_worker(worker)
{}


void ClientThread::run()
{
  std::ostringstream ss;
  ss << m_worker->rdbuf();
  std::string s =ss.str();
  std::string c =m_worker->getPeerAddr();

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
          tntdb::Row row = *result.begin();
          int a = -1;
          row[0].get(a);  // read column 0 into variable a
          if(a == 0 || a == -1)
          {
              // this host is not enabled
              std::cout << "Not allowed:" << c << std::endl;
              return;
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

          TiXmlElement* el_for_devices = doc.FirstChildElement("devices");
          if(el_for_devices)
          {
              bool timestamp_sent = true;
              const char* timestamp_attr = el_for_devices->Attribute("timestamp");
              if(timestamp_attr == 0)
              {
                  timestamp_attr = "extract(epoch from now())";
                  timestamp_sent = false;
              }
              TiXmlElement* el_for_device = el_for_devices->FirstChildElement("device");

              std::stringstream qbr;
              qbr << "insert into " <<
                     "sinf01_disk_statistics(disk_stat_disk_id, disk_stat_free_space, disk_stat_measurement_time) " <<
                     "values( :v1, :v2, to_timestamp(" << timestamp_attr << "))";

              tntdb::Statement prepared_insertion = conn.prepare(qbr.str());

              // first run if the host needs initialization
              if(host_need_initialization)
              {
                  while(el_for_device)
                  {
                      std::string disk_part_name;
                      std::string disk_mountpoint;
                      uint64_t total_disk_space = 0;
                      uint64_t free_disk_space = 0;
                      std::string fs_type;
                      std::string disk_label;

                      const char* attr = el_for_device->Attribute("name");
                      if(attr) { disk_part_name = attr; }

                      attr = el_for_device->Attribute("mountpt");
                      if(attr) { disk_mountpoint = attr; }

                      attr = el_for_device->Attribute("fs_type");
                      if(attr) { fs_type = attr; }

                      attr = el_for_device->Attribute("total_bytes");
                      if(attr) { total_disk_space = atoll(attr); }

                      attr = el_for_device->Attribute("free_bytes");
                      if(attr) { free_disk_space = atoll(attr); }

                      attr = el_for_device->Attribute("label");
                      if(attr) { disk_label = attr; }

                      // create an insert statement into the disk table with the data
                      // above depending on the host_need_initialization, use the host_id

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

                      el_for_device = el_for_device->NextSiblingElement("device");

                  }
              }

              // second run: to actually insert the data in the statistic table and do the calculations based on the data we get
              int smallestPercentage = 100;
              int host_status = 0;
              std::string host_status_explanation("All OK");
              el_for_device = el_for_devices->FirstChildElement("device");
              while(el_for_device)
              {
                  std::string disk_part_name;
                  uint64_t free_disk_space = 0;
                  uint64_t total_disk_space = 0;

                  const char* attr = el_for_device->Attribute("name");
                  if(attr) { disk_part_name = attr; }

                  attr = el_for_device->Attribute("free_bytes");
                  if(attr) { free_disk_space = atoll(attr); }

                  attr = el_for_device->Attribute("total_bytes");
                  if(attr) { total_disk_space = atoll(attr); }

                  // find the disk ID from sinf01_disk table:
                  int found_disk_id = 0;
                  {
                  std::stringstream ss;
                  ss << "select disk_id from sinf01_disk where host_id=" << host_id <<" and disk_physical_id='"<< disk_part_name <<"'";
                  std::string s= ss.str();
                  //std::cout << s << std::endl;
                  tntdb::Result result = conn.select(s);
                  tntdb::Row row = *result.begin();
                  row[0].get(found_disk_id);  // read column 0 into variable a
                  if(found_disk_id == 0)      //  this means, the host was not in the database till now
                  {
                      std::cout << "no disk id found for " <<host_id << " and " << disk_part_name;
                      return;
                  }
                  }
                  // and here finally insert the data into the statistics table
                  prepared_insertion.set("v1", found_disk_id).set("v2", free_disk_space).execute();
                  if(total_disk_space > 0)
                  {
                      // do some statistics calculation
                      if(smallestPercentage > 100 * free_disk_space / total_disk_space)
                      {
                          smallestPercentage = 100 * free_disk_space / total_disk_space;
                      }
                  }

                  if(smallestPercentage < 15 && host_status == 0)
                  {
                      // flip the host status only if the remaining space is > 1GB because <1GB as 15 percent can be only on a pretty small hard disk and does not matter
                      if(free_disk_space > 1073741824)
                      {
                          host_status = 1;
                          std::stringstream ss;
                          ss << "Warning: " << disk_part_name << " has only " << free_disk_space/1073741824 << "GB free, out of " << total_disk_space/1073741824 << "GB representing " << smallestPercentage <<"%";
                          host_status_explanation = ss.str();
                      }
                  }

                  if(smallestPercentage < 5  && host_status == 0)
                  {
                      // flip the host status only if the remaining space is > 1GB
                      if(free_disk_space > 1073741824)
                      {
                          host_status = 2;
                          std::stringstream ss;
                          ss << "Critical: " << disk_part_name << " has only " << free_disk_space/1073741824 << "GB free, out of " << total_disk_space/1073741824<< "GB representing " << smallestPercentage <<"%";
                          host_status_explanation = ss.str();
                      }
                  }

                  el_for_device = el_for_device->NextSiblingElement("device");
              }

              // and update the "host" table with the last status
              std::stringstream ss;
              ss << "update sinf01_host set host_last_status=" << host_status << ", host_last_status_text='" << host_status_explanation << "' where host_id = " << host_id;
              conn.execute(ss.str());

          } // devices node was found
          else
          {
              std::cout << "no root" << std::endl;
          }

          conn.close();
      }
  }
  catch (const std::exception& e)
  {
      std::cerr << e.what() << std::endl;
  }
  m_worker->close();
}

