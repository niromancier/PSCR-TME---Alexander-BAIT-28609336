#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>

#define MAX_MSG_SIZE 128

void handle_client(int sock) {
  char msg[MAX_MSG_SIZE] = {0};
  while (1) {
    std::memset(msg, 0, MAX_MSG_SIZE);
    if (recvfrom(sock, msg, MAX_MSG_SIZE, 0, NULL, NULL) < 0) {
      perror("recvfrom");
      exit(1);
    }
    std::cout << msg << std::endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "usage: " << *argv << " IP_ADDRESS PORT NAME" << std::endl;
    exit(1);
  }

  int sock;
  struct sockaddr_in addr;

  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(std::stoi(argv[2]));
  addr.sin_family = AF_INET;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  struct ip_mreq group;
  group.imr_multiaddr.s_addr = inet_addr(argv[1]);
  group.imr_interface.s_addr = INADDR_ANY;

  int reuse = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(reuse)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group,
                 sizeof(group)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  std::thread t(handle_client, sock);

  char buffer[MAX_MSG_SIZE] = {0};
  while (1) {
    std::memset(buffer, 0, MAX_MSG_SIZE);
    std::cin.getline(buffer, MAX_MSG_SIZE);
    if (sendto(sock, buffer, MAX_MSG_SIZE, 0, (struct sockaddr *)&addr,
               sizeof(addr)) < 0) {
      perror("sendto");
      exit(1);
    }
  }

  t.join();
  close(sock);

  return 0;
}
