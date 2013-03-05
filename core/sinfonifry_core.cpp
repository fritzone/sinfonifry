
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
    std::cout << s;

    std::cout << "here" << std::endl; 
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
      std::cout << "new thread: " << std::endl;
      (new ClientThread(worker))->start();
 
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

