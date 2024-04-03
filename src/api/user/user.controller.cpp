#include "user.controller.h"
#include "user.service.h"

user_controller::user_controller(RouteManager &r, env &env_): r(r), env_(env_) {
  user_controller::setRoot("/user");
  user_service userService;

  addRoute("GET", "hello")
  ([](const crow::request &req) {
    return crow::response(200, "hello");
  });

  addRoute("POST", "/")
  ([&userService](const crow::request &req) {
    user_service::User user;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      int id;
      if (json.has("id")) {
        id = json["id"].i();
      } else {
        id = 0;
      }
      user_service::User data(id, json["name"].s());
      user = userService.create(data);
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, user.toJson().dump());
  });

  addRoute("POST", "/bulk")
  ([this, &userService](const crow::request &req) {
    std::vector<user_service::User> newUsers;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      for (const auto &user : json) {
        int id = 0;
        if (user.has("id")) {
          id = user["id"].i();
        }
        std::string name = "No name provided";
        if (user.has("name")) {
          name = user["name"].s();
          newUsers.push_back(userService.create(user_service::User(id, name)));
        } 
      }
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    std::vector<crow::json::wvalue> jsonResponse;
    for (const auto &user : newUsers) {
      jsonResponse.push_back(user.toJson());
    }

    return crow::response(200, crow::json::wvalue(jsonResponse).dump());
  });

  addRoute("GET", "/<int>")
  ([&userService](int id) {
    try {
      user_service::User user = userService.read(id);
      if (user.id == 0) {
        return crow::response(404, "User not found");
      }
      return crow::response(200, user.toJson().dump());
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
  });

  addRoute("GET", "/")
  ([&userService](const crow::request &req) {
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

      std::vector<user_service::User> users;
      if (!ids.empty()) {
        users = userService.read(ids);
      } else {
        users = userService.read();
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

  addRoute("PATCH", "/")
  ([&userService](const crow::request &req) {
    user_service::User newUser;
    try {
      crow::json::rvalue json = crow::json::load(req.body);
      user_service::User user(json["id"].i(), json["name"].s());
      newUser = userService.update(user);
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, newUser.toJson().dump());
  });

  addRoute("DELETE", "/<int>")
  ([&userService](int id) {
    user_service::User user;
    try {
      user = userService.remove(id);
      if (user.id == 0) {
        return crow::response(404, "User not found");
      }
    } catch (const std::exception &e) {
      return crow::response(500, e.what());
    }
    return crow::response(200, user.toJson().dump());
  });

  addRoute("DELETE", "/")
  ([&userService](const crow::request &req) {
    std::vector<user_service::User> users;
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
        users = userService.remove(ids);
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
