#ifndef APP_SERVICE_H
#define APP_SERVICE_H

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <mysql/mysql.h>

struct User;

class app_service {
public:
  app_service(soci::session& sql) : sql(sql) {}

  void create(const User& data);
  void create(const std::vector<User>& data);
  User read(std::vector<int> ids);
  User read(int id);
  User read();
  void update(const User& user);
  void remove(int id);
  void remove(std::vector<int> ids);

  crow::mustache::rendered_template home();
private:
  soci::session& sql;
};

#endif // APP_SERVICE_H
