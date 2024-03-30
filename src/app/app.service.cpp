#include "app.service.h"

void app_service::create(const User &data) {
  sql << "INSERT INTO users (id, name) VALUES (:id, :name)", soci::use(data.id), soci::use(data.name);
};

void app_service::create(const std::vector<User>& data) {
  std::vector<int> ids;
  std::vector<std::string> names;
  for (auto &user : data) {
    ids.push_back(user.id);
    names.push_back(user.name);
  }
  sql << "INSERT INTO users (id, name) VALUES (:id, :name)", soci::use(ids), soci::use(names);
};

std::vector<app_service::User> app_service::read(std::vector<int> ids) {
  std::vector<app_service::User> users;
  soci::rowset<soci::row> rs = (sql.prepare << "SELECT id, name FROM users WHERE id IN :ids", soci::use(ids));
  for (soci::row &r : rs) {
    app_service::User user;
    r.get<int>(0, user.id);
    r.get<std::string>(1, user.name);
    users.push_back(user);
  }
  return users;
};

std::vector<app_service::User> app_service::read() {
  std::vector<User> users;
  soci::rowset<soci::row> rs = (sql.prepare << "SELECT id, name FROM users");
  for (soci::row &r : rs) {
    User user;
    r.get<int>(0, user.id);
    r.get<std::string>(1, user.name);
    users.push_back(user);
  }
  return users;
} 

app_service::User app_service::app_service::read(int id) {
  User user;
  soci::indicator ind;
  sql << "SELECT id, name FROM users WHERE id = :id", soci::use(id),
      into(user.id, ind), into(user.name, ind);

  if (ind == soci::i_null) {
    throw std::runtime_error("User not found");
  }

  return user;
}

void app_service::app_service::update(const User &user) {
  sql << "UPDATE users SET name = :name WHERE id = :id", soci::use(user.name),
      soci::use(user.id);
}

void app_service::app_service::remove(int id) {
  sql << "DELETE FROM users WHERE id = :id", soci::use(id);
}

void app_service::remove(std::vector<int> ids) {
  sql << "DELETE FROM users WHERE id IN :ids", soci::use(ids);
};

crow::mustache::rendered_template app_service::home() {
  auto page = crow::mustache::load("base.html");
  crow::mustache::context ctx;
  ctx["PUBLIC_URL"] = "/static";
  ctx["content"] = crow::mustache::load("child.html").render_string();
  return page.render(ctx);
}
