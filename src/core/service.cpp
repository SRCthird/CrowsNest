#include "service.h"
#include "utils.h"

crow::mustache::rendered_template service::home() {
  auto page = crow::mustache::load("base.html");
  crow::mustache::context ctx;
  ctx["PUBLIC_URL"] = "/static";
  ctx["content"] = crow::mustache::load("child.html").render_string();
  return page.render(ctx);
}

void service::staticDir(const crow::request &req, crow::response &res,std::string fileName) {
  std::ifstream in("static/" + fileName, std::ifstream::in | std::ifstream::binary);
  if (in) {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    res.body = contents.str();
    res.add_header("Content-Type", getContentType(fileName));
  } else {
    res.code = 404;
    res.body = "File not found";
  }
  res.end();
}

void service::apiCall(const crow::request &req, crow::response &res) {
  res.code = 401;
  res.add_header("line", "RPI00001");
  res.end();
}
