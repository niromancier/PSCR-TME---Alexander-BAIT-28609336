#include <signal.h>
#include <time.h>
#include <iostream>

size_t pv = 3;

void randsleep() {
  int r = rand();
  double ratio = (double)r / (double)RAND_MAX;
  struct timespec tosleep;
  tosleep.tv_sec = 0;
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio * 700000000;
  struct timespec remain;
  while (nanosleep(&tosleep, &remain) != 0) {
    tosleep = remain;
  }
}

void handler(int sig) {
  (void)sig;  // clear warning
  pv--;
  printf("[%d] touché, il reste : %ld pv\n", getpid(), pv);
  if (!pv) {
    puts("DEAD");
    exit(1);
  }
}

void coup_pare(int sig) {
  (void)sig;  // clear warning
  printf("[%d] parré, il reste : %ld pv\n", getpid(), pv);
}

struct sigaction sa;
sigset_t set;

void defense_luke() {
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = coup_pare;
  sigaction(SIGINT, &sa, nullptr);

  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask(SIG_BLOCK, &set, nullptr);
  randsleep();
  sigsuspend(&set);
}

void defense() {
  signal(SIGINT, SIG_IGN);
  randsleep();
}

void attaque(pid_t adversaire) {
  signal(SIGINT, handler);
  printf("pid adversaire: %d\n", adversaire);
  if (kill(adversaire, SIGINT)) {
    puts("GAGNÉ");
    exit(0);
  }
  randsleep();
}

void combat(pid_t adversaire, bool luke = false) {
  while (true) {
    luke ? defense_luke() : defense();
    attaque(adversaire);
  }
}

int main() {
  pid_t p = fork();
  (!p) ? combat(getppid(), true) : combat(p);
  return 0;
}
