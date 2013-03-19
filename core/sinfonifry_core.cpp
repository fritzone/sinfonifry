
#include <exception>
#include <iostream>
#include "client_thread.h"

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

