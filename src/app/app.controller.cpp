#include "app.controller.h"
#include "app.service.h"

app_controller::app_controller(crow::SimpleApp& app): app(app), env_(".env") {
  app_controller::setRoot("/");
  
  //soci::session sql("sqlite3", env_.getDatabaseConnection());
  soci::session sql("sqlite3", "sqlite3.db");
  app_service app_service(sql);

  app.route_dynamic(app_controller::route())
  ([&app_service]() { 
    return app_service.home(); 
  });

  app.route_dynamic(app_controller::route() + "/get")
  ([&app_service]() {
    std::vector<crow::json::wvalue> json_users;
    std::vector<app_service::User> users = app_service.read();

    for (const app_service::User& user : users) {
      json_users.push_back(user.to_json());
    }

    crow::json::wvalue final = std::move(json_users);

    return crow::response(std::move(final));
  });

};

