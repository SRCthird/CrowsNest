#include "controller.h"
#include "../app/app.controller.h"

int main() {
  crow::SimpleApp app;
  env env(".env");
  
  // Import Controllers
  controller ctrl(app, env);
  app_controller app_ctrl(app, env);
  // End Import Controllers
  
  ctrl.run();
}
