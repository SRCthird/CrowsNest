#include "app_controller.h"
#include "app_service.h"

app_controller::app_controller(crow::SimpleApp& app): app(app), env_(".env") {
  app_controller::setRoot("/");

  app_service appService;

  app.route_dynamic(app_controller::route())
  ([&appService]() { 
    return appService.home(); 
  });
};
