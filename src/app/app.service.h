#ifndef APP_SERVICE_H
#define APP_SERVICE_H 

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>

class app_service {
public:

  app_service() {}

  crow::mustache::rendered_template home() {
    auto page = crow::mustache::load("base.html");
    crow::mustache::context ctx;
    ctx["PUBLIC_URL"] = "/static";
    ctx["content"] = crow::mustache::load("child.html").render_string();
    return page.render(ctx);
  }

private:
};

#endif // APP_SERVICE_H  
