#ifndef USER_SERVICE_H
#define USER_SERVICE_H 

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <soci/sqlite3/soci-sqlite3.h>

class user_service {
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

  user_service() {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    try {
      verifyConnection(sql);
      sql << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)";
    } catch (const std::exception& e) {
      std::cerr << "Error creating table: " << e.what() << std::endl;
    }
    sql.close();
  }

  User create(const User& user) {
    soci::session sql(soci::sqlite3, "db=sqlite3.db timeout=2 shared_cache=true");
    int lastId = user.id;
    try {
      verifyConnection(sql);
      if (user.id == 0) {
        sql << "INSERT INTO users (name) VALUES (:name)", soci::use(user.name);
        sql << "SELECT last_insert_rowid()", soci::into(lastId);
      } else {
        sql << "INSERT INTO users (id, name) VALUES (:id, :name)", soci::use(user.id), soci::use(user.name);
      }
    } catch (const std::exception& e) {
      std::cerr << "Error executing query: " << e.what() << std::endl;
    }
    sql.close();
    User newUser = this->read(lastId);
    return newUser;
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

#endif // USER_SERVICE_H  
