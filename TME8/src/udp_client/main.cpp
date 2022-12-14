#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

#define MAX_MSG_SIZE 128

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << *argv << " IP_ADDR PORT" << std::endl;
    exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(std::stoi(argv[2]));

  char buf[MAX_MSG_SIZE] = {0};
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  socklen_t server_size = sizeof(addr);
  std::cin.getline(buf, MAX_MSG_SIZE);
  sendto(s, buf, std::strlen(buf), MSG_CONFIRM, (struct sockaddr *)&addr,
         server_size);

  memset(buf, 0, MAX_MSG_SIZE);
  recvfrom(s, buf, MAX_MSG_SIZE, MSG_WAITALL, (struct sockaddr *)&addr,
           &server_size);
  std::cout << buf << std::endl;

  return 0;
}
