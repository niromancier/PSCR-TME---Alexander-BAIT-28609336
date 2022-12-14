#include <iostream>

#include "UDPserver.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << *argv << " PORT" << std::endl;
    exit(1);
  }
  int port = std::stoi(argv[1]);
  pr::UDPserver srv(port);
  srv.startServer();
  return 0;
}
