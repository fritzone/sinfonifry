#include <tnt/tntnet.h>

int main(int argc, char* argv[])
{
  try
  {
    tnt::Tntnet app;
    app.listen("0.0.0.0", 8000);
    app.mapUrl("^/$","sinfonifry@sinfonifry");
    app.mapUrl("^/([^.]+)(\\..+)?","$1@sinfonifry");
    app.mapUrl("/sinfonifry", "sinfonifry@sinfonifry");
    app.mapUrl("/(.*).png", "images@sinfonifry");
    app.mapUrl("/(.*).js", "jscript@sinfonifry");
    app.mapUrl("/(.*).css", "css@sinfonifry");

    app.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
