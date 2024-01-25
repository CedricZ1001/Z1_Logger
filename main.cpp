#include<iostream>
using namespace std;
#include "Logger.h"
int main(){
  Logger::instance()->open("./test.log");
  Logger::instance()->log(Logger::DEBUG,__FILE__,__LINE__,"hello world");
  return 0;
}