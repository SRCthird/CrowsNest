#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "../core/utils.h"
#include "../core/environment.h"
#include "app_service.h"
#include <iostream>
#include <sstream>
#include <string>

class app_controller {
public:
  crow::SimpleApp& app;
  app_controller(crow::SimpleApp& app);

  void setRoot(std::string root) {
      this->root = root;
  }
  const std::string route(const std::string append) {
      return this->root + append;
  }
  const std::string route() {
      return this->root;
  }

private:
  std::string root = "/";
  env env_;
};

#endif // APP_CONTROLLER_H
