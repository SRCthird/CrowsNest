#ifndef SERVICE_H
#define SERVICE_H
#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>

class service {
public:
  crow::mustache::rendered_template home();
  void staticDir(const crow::request &req, crow::response &res, std::string fileName);
  void apiCall(const crow::request &req, crow::response &res);
private:
};

#endif // SERVICE_H
