#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

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


/**
 * @brief The ClientThread class represents a client which connects to the
 *        core and periodically sends in data which is the inserted into the
 *        database
 */
class ClientThread : public cxxtools::DetachedThread
{
public:
    ClientThread(cxxtools::net::iostream* worker);

protected:
    void run();

private:
  cxxtools::net::iostream* m_worker;
};

#endif // CLIENT_THREAD_H
