#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "environment.h"
#include "service.h"
#include <iostream>
#include <string>

class controller {
public:
  crow::SimpleApp &app;
  env &env_;
  controller(crow::SimpleApp &app, env &env) : app(app), env_(env) {
    port = env.getPort();
    hostname = env.getHostname();
    if (env.getHttps()) {
      if (!env.getCert().empty() || !env.getKey().empty()) {
        std::cout << "Cert and key found" << std::endl;
        app.ssl_file(env.getCert(), env.getKey());
      } else if (!env.getPem().empty()) {
        std::cout << "key found" << std::endl;
        app.ssl_file(env.getPem());
      }
    }

    controller::setRoot("/");

    service service;

    app.route_dynamic(controller::route("favicon.ico"))(
        [&service](const crow::request &req, crow::response &res) {
          return service.staticDir(req, res, "favicon.ico");
        });

    app.route_dynamic(controller::route("manifest.json"))(
        [&service](const crow::request &req, crow::response &res) {
          return service.staticDir(req, res, "manifest.json");
        });

    app.route_dynamic(controller::route("logo192.png"))(
        [&service](const crow::request &req, crow::response &res) {
          return service.staticDir(req, res, "logo192.png");
        });

    app.route_dynamic(controller::route("logo512.png"))(
        [&service](const crow::request &req, crow::response &res) {
          return service.staticDir(req, res, "logo512.png");
        });
  };

  void setRoot(std::string root) { this->root = root; }
  std::string route(const std::string append) {
    return this->root + append;
  }
  std::string route() { return this->root; }
  inline void run() { app.port(port).multithreaded().run(); };

private:
  std::string root = "/";
  int port;
  std::string hostname;
};

#endif // CONTROLLER_H
