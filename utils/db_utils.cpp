#include "db_utils.h"
#include <tntdb/connection.h>
#include "configuration.h"

namespace sinfonifry
{

using namespace std;

std::string get_master_connection_string(const Configuration& conf)
{
	// "postgresql:dbname=sinfonifry user=sinfonifry password=s1nf0n1fry"
	string s = "postgresql:dbname=";
    string db_host = conf.getConfigSetting("core", "db_host", "127.0.0.1");
    string db_user = conf.getConfigSetting("core", "db_user", "sinfonifry");
    string db_name = conf.getConfigSetting("core", "db_name", "sinfonifry");
    string db_pass = conf.getConfigSetting("core", "db_pass", "s1nf0n1fry");
	s += db_name;
	s += " user=";
	s += db_user;
	s += " password=";
	s += db_pass;
	s += " host=";
	s += db_host;
	return s;
}

}
