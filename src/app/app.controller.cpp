#include "app.controller.h"
#include "app.service.h"

app_controller::app_controller(crow::SimpleApp &app, env &env_): app(app), env_(env_) {
  app_controller::setRoot("/");
  app_service appService;

  app.route_dynamic(app_controller::route())
  ([&appService](const crow::request &req) {
    return crow::response(200, appService.home());
  });
}
