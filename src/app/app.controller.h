#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "../core/environment.h"
#include <soci/mysql/soci-mysql.h>
#include <string>

class app_controller {
public:
  crow::SimpleApp& app;
  
  app_controller(crow::SimpleApp& app);

  void setRoot(std::string root) {
      this->root = root;
  }
  std::string route(const std::string append) {
      return this->root + append;
  }
  std::string route() {
      return this->root;
  }

private:
  std::string root = "/";
  env env_;
};

#endif // APP_CONTROLLER_H
