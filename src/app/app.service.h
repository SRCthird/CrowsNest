#ifndef APP_SERVICE_H
#define APP_SERVICE_H 

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>
#include <vector>
#include "../api/user/user.service.h"

class app_service {
public:

  app_service() {}
  user_service userSvc;

  const crow::mustache::rendered_template home() {
    auto page = crow::mustache::load("base.html");
    auto child = crow::mustache::load("usersList.html");

    crow::mustache::context ctx;
    ctx["PUBLIC_URL"] = "/static";
    std::vector<user_service::User> users = userSvc.read();

    for (auto& user : users) {
      ctx["users"][user.id] = crow::mustache::context{
        {"id", user.id},
        {"name", user.name}
      };
    }
    ctx["content"] = child.render_string(ctx);

    return page.render(ctx);
  }

private:
};

#endif // APP_SERVICE_H  
