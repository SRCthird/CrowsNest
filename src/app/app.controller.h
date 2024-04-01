#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "../core/environment.h"
#include <soci/connection-pool.h>
#include <soci/mysql/soci-mysql.h>
#include <string>

class app_controller {
public:
  app_controller(crow::SimpleApp &app, env &env_);
  void setRoot(std::string root) { this->root = root; }
  std::string route(const std::string append) { 
    if (append[0] == '/') {
      return this->root + append.substr(1); 
    }
    return this->root + append; 
  }
  std::string route() { return this->root; }

private:
  crow::SimpleApp &app;
  env &env_;
  std::string root = "/";
};

#endif // APP_CONTROLLER_H
