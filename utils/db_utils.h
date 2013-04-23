#ifndef DB_UTILS_H_
#define DB_UTILS_H_

#include <string>
#include <tntdb/connection.h>

class Configuration;

namespace sinfonifry
{

/**
 * Returns the connection string where the core component is supposed to
 * connect in order to perform basic database functionality. This must not be
 * the same database where the plugins will connect.
 */
std::string get_master_connection_string(const Configuration &conf);

}

#endif /* DB_UTILS_H_ */
