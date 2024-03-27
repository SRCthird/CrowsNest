#include "app.controller.h"
#include "app.service.h"

app_controller::app_controller(crow::SimpleApp& app): app(app), env_(".env") {
  app_controller::setRoot("/");

  soci::session sql(soci::mysql, env_.getDatabaseConnection());
  app_service service(sql);

  app.route_dynamic(app_controller::route())
  ([&service]() { 
    return service.home(); 
  });
};
