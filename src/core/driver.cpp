#include "controller.h"
#include "../user/user.controller.h"

int main() {
  crow::SimpleApp app;
  env env(".env");
  
  // Import Controllers
  controller ctrl(app, env);
  user_controller user_ctrl(app, env);
  // End Import Controllers
  
  ctrl.run();
}
