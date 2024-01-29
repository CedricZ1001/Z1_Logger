#include<iostream>
using namespace std;
#include "Logger.h"


int main(){
  Logger::instance()->open("./log/test.log");
  //Logger::instance()->level(Logger::INFO);
  Logger::instance()->max(1024);

  debug("Macro");
  info("info");
  warn("warn");
  error("error");
  fatal("fatal");

  return 0;
}