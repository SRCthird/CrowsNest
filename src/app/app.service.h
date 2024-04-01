#ifndef APP_SERVICE_H
#define APP_SERVICE_H 

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <soci/sqlite3/soci-sqlite3.h>

class app_service {
public:
  struct User {
      int id;
      std::string name;

      User() : id(0), name("") {}
      User(int id, std::string name) : id(id), name(name) {}

      crow::json::wvalue toJson() const {
          crow::json::wvalue response;
          response["id"] = id;
          response["name"] = name;
          return response;
      }
  };

  app_service() {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      sql << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)";
      std::cout << "Table 'users' created or already exists." << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Error creating table: " << e.what() << std::endl;
    }
    sql.close();
  }

  crow::mustache::rendered_template home() {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    auto page = crow::mustache::load("base.html");
    crow::mustache::context ctx;
    ctx["PUBLIC_URL"] = "/static";
    ctx["content"] = crow::mustache::load("child.html").render_string();
    return page.render(ctx);
  }

  User create(const User& user) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      sql << "INSERT INTO users (id, name) VALUES (:id, :name)", soci::use(user.id), soci::use(user.name);
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    User newUser = this->read(user.id);
    return newUser;
  }

   std::vector<User> create(const std::vector<User>& users) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    std::vector<int> ids;
    try {
      verifyConnection(sql);
      std::vector<std::string> names;
      for (const auto &user : users) {
        ids.push_back(user.id);
        names.push_back(user.name);
      }
      sql << "INSERT INTO users (id, name) VALUES (:id, :name)", soci::use(ids), soci::use(names);
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    std::vector<User> newUsers = this->read(ids);
    return newUsers;
  };

  User read(int id) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      User user;
      soci::indicator ind;
      sql << "SELECT id, name FROM users WHERE id = :id", soci::use(id),
          soci::into(user.id, ind), soci::into(user.name, ind);
      if (ind == soci::i_null) {
        throw std::runtime_error("User not found");
      }
      sql.close();
      return user;
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    return User();
  }

  std::vector<User> read(const std::vector<int>& ids) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    std::vector<User> users;
    try {
      verifyConnection(sql);
      for (const auto &id : ids) {
        User user;
        soci::indicator ind;
        sql << "SELECT id, name FROM users WHERE id = :id", soci::use(id), soci::into(user.id, ind), soci::into(user.name, ind);
        if (ind == soci::i_null) {
          throw std::runtime_error("User not found");
        }
        users.push_back(user);
      }
      sql.close();
      return users;
    } catch (const std::exception& e) {
        std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    return std::vector<User>();
  };

  std::vector<User> read() {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      std::vector<User> users;
      soci::rowset<soci::row> rs = (sql.prepare << "SELECT id, name FROM users");

      for (const auto& row : rs) {
          User user;
          user.id = row.get<int>(0);
          user.name = row.get<std::string>(1);
          users.push_back(user);
      }

      sql.close();
      return users;
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    return std::vector<User>();
  }

  User update(const User& user) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      sql << "UPDATE users SET name = :name WHERE id = :id", soci::use(user.name), soci::use(user.id);
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    User newUser = this->read(user.id);
    return newUser;
  }

  User remove(int id) {
    User user = this->read(id);
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      sql << "DELETE FROM users WHERE id = :id", soci::use(id);
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    return user;
  }

  std::vector<User> remove(const std::vector<int>& ids) {
    std::vector<User> users;
    users = this->read(ids);
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      for (const auto &id : ids) {
        sql << "DELETE FROM users WHERE id = :id", soci::use(ids);
      }
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    return users;
  }

private:
  void verifyConnection(soci::session& sql) {
    if (!sql.is_connected()) {
      throw std::runtime_error("Database is not connected.");
    }
  }
};

#endif // APP_SERVICE_H
