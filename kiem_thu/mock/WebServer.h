#pragma once
#include "Arduino.h"
enum HTTPMethod { HTTP_GET, HTTP_POST };
#define BASIC_AUTH 0
struct WebServer {
  std::map<std::string, std::function<void()>> h[2];
  std::map<std::string, std::string> args, hdrs;
  int lastCode = 0; std::string lastBody;
  WebServer(int) {}
  void on(const char *u, HTTPMethod m, std::function<void()> f) { h[m][u] = f; }
  void onNotFound(std::function<void()>) {}
  void begin() {} void handleClient() {}
  void collectHeaders(const char **, int) {}
  bool authenticate(const char *, const char *) { return true; }
  void requestAuthentication(int, const char *) {}
  String header(const char *n) { return String(hdrs[n]); }
  String arg(const char *n) { return String(args[n]); }
  bool hasArg(const char *n) { return args.count(n) > 0; }
  void send(int c, const char *, const char *b) { lastCode = c; lastBody = b; }
  void send_P(int c, const char *, const char *b) { lastCode = c; lastBody = b; }
  void sendHeader(const char *, const char *) {}
  // dùng cho kiểm thử: mô phỏng một yêu cầu HTTP
  void req(HTTPMethod m, const char *u, std::map<std::string, std::string> a = {}) {
    args = a; hdrs["X-Req"] = "1"; lastCode = -1; lastBody.clear();
    h[m].at(u)();
  }
};
