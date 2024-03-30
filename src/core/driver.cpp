#include "controller.h"
#include "../app/app.controller.h"

int main() {
  crow::SimpleApp app;
  
  // Import Controllers
  controller ctrl(app);
  app_controller app_ctrl(app);

  std::cout << "App Controller Loaded" << std::endl;
  // End Import Controllers
  
  ctrl.run();
}
