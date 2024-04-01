#include "app.controller.h"
#include "app.service.h"

app_controller::app_controller(crow::SimpleApp &app, env &env_)
    : app(app), env_(env_) {
  app_controller::setRoot("/");


  app_service appService;

  /*app.route_dynamic(app_controller::route()).methods("GET"_method)
  ([&appService]() { 
    return appService.home(); 
  });*/

  app.route_dynamic(app_controller::route("")).methods("POST"_method)
  ([&appService](const crow::request &req) {
    app_service::User user;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      app_service::User data(json["id"].i(), json["name"].s());
      int id;
      user = appService.create(data);
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, user.toJson().dump());
  });

  app.route_dynamic(app_controller::route("bulk")).methods("POST"_method)
  ([&appService](const crow::request &req) {
    std::vector<app_service::User> users;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      std::vector<app_service::User> data;
      for (const auto &user : json) {
        data.emplace_back(user["id"].i(), user["name"].s());
      }
      users = appService.create(data);
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    std::vector<crow::json::wvalue> jsonResponse;
    for (const auto &user : users) {
      jsonResponse.push_back(user.toJson());
    }

    return crow::response(200, crow::json::wvalue(jsonResponse).dump());
  });

  app.route_dynamic(app_controller::route("<int>")).methods("GET"_method)
  ([&appService](int id) {
    try {
      app_service::User user = appService.read(id);
      if (user.id == 0) {
        return crow::response(404, "User not found");
      }
      return crow::response(200, user.toJson().dump());
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
  });

  app.route_dynamic(app_controller::route()).methods("GET"_method)
  ([&appService](const crow::request &req) {
    try {
      std::vector<int> ids;
      const char* idParam = req.url_params.get("id");
      if (idParam != nullptr) {
        std::stringstream ss(idParam);
        std::string id;
        while (std::getline(ss, id, ',')) {
          ids.push_back(std::stoi(id));
        }
      }

      std::vector<app_service::User> users;
      if (!ids.empty()) {
        users = appService.read(ids);
      } else {
        users = appService.read();
      }

      std::vector<crow::json::wvalue> jsonResponse;
      for (const auto &user : users) {
        jsonResponse.push_back(user.toJson());
      }

      return crow::response(200, crow::json::wvalue(jsonResponse).dump());
    } catch (const std::exception &e) {
      return crow::response(404, e.what());
    }
  });

  app.route_dynamic(app_controller::route()).methods("PUT"_method)
  ([&appService](const crow::request &req) {
    app_service::User newUser;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      app_service::User user(json["id"].i(), json["name"].s());
      newUser = appService.update(user);
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, newUser.toJson().dump());
  });

  app.route_dynamic(app_controller::route("<int>")).methods("DELETE"_method)
  ([&appService](int id) {
    app_service::User user;
    try {
      user = appService.remove(id);
      if (user.id == 0) {
        return crow::response(404, "User not found");
      }
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, user.toJson().dump());
  });

  app.route_dynamic(app_controller::route()).methods("DELETE"_method)
  ([&appService](const crow::request &req) {
    std::vector<app_service::User> users;
    try {
      std::vector<int> ids;
      const char* idParam = req.url_params.get("id");
      if (idParam != nullptr) {
        std::stringstream ss(idParam);
        std::string id;
        while (std::getline(ss, id, ',')) {
          ids.push_back(std::stoi(id));
        }
      }
      if (!ids.empty()) {
        users = appService.remove(ids);
      } else {
        throw std::runtime_error("No ids provided");
      }
    } catch (const std::runtime_error &e) {
      return crow::response(400, e.what());
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    std::vector<crow::json::wvalue> jsonResponse;
    for (const auto &user : users) {
      jsonResponse.push_back(user.toJson());
    }

    return crow::response(200, crow::json::wvalue(jsonResponse).dump());
  });
}
