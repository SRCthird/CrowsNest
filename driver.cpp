#include "src/core/controller.h"
#include "src/app/app_controller.h"

int main() {
  crow::SimpleApp app;
  
  // Import Controllers
  controller ctrl(app);
  app_controller app_ctrl(app);
  // End Import Controllers
  
  ctrl.run();
}
