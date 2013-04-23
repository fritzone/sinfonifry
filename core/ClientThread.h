#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <cxxtools/thread.h>
#include <cxxtools/net/tcpstream.h>

class Configuration;

class ClientThread : public cxxtools::DetachedThread
{
public:
  ClientThread(cxxtools::net::iostream* worker, const Configuration& conf) :
    m_worker(worker), m_conf(conf)
  {}

private:

  /* return true if the host which just connected to us is allowed in the DB to connect */
  bool hostIsAllowed(const std::string& c);

  /* return true if the host needs initialization */
  bool hostNeedsInit(const std::string& ip);


protected:
  void run();

private:
  cxxtools::net::iostream* m_worker;
  const Configuration& m_conf;
};

#endif // CLIENTTHREAD_H
