#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H 

#include "../../core/environment.h"
#include "../../core/routeManager.h"
#include <soci/connection-pool.h>
#include <soci/mysql/soci-mysql.h>
#include <string>

class user_controller {
public:
  user_controller(RouteManager &r, env &env_);
  void setRoot(std::string root) { this->root += root; }
  std::string route(const std::string append) { 
    if (this->root.back() == '/' && append[0] == '/') {
      return this->root + append.substr(1); 
    } else if (this->root.back() != '/' && append[0] != '/') {
      return this->root + "/" + append; 
    }
    return this->root + append; 
  }
  std::string route() { return this->root; }

private:
  RouteManager &r;
  env &env_;
  std::string root = "/api";
};

#endif // USER_CONTROLLER_H 
