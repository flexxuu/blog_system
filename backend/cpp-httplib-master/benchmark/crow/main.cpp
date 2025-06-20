#include "crow_all.h"

class CustomBLogger : public crow::ILogHandler {
public:
  void log(std::string, crow::LogLevel) {}
};

int main() {
  CustomBLogger logger;
  crow::logger::setHandler(&logger);

  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() { return "Hello world!"; });

  app.port(8080).multithreaded().run();
}
