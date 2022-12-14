#include <iostream>
#include <sys/wait.h>
#include <signal.h>

bool a = false;
bool c = false;

void sig_handler(int sig) {
  switch (sig) {
    case SIGCHLD:
      alarm(0);
      c = true;
      break;
    case SIGALRM:
      a = true;
      break;
  }
}

/* Question 7
int wait_till_pid(pid_t pid) {
  pid_t p;

  signal(SIGALRM, sig_handler);
  alarm(sec);
  while ((p = wait(nullptr)) != pid)
    if (p < 0) return -1;
  return pid;
}
*/

// Question 8
int wait_till_pid(pid_t pid, int sec) {
  pid_t p;
  signal(SIGALRM, sig_handler);
  signal(SIGCHLD, sig_handler);
  alarm(sec);
  while (true) {
    if (a) return 0;
    if (c) {
      c = false;
      p = wait(nullptr);
      if (p < 0) return -1;
      if (p == pid) return pid;
    }
    sleep(sec);
  }
  return 0;
}

int main() {
  pid_t pid = fork();
  if (!pid) { while (true) continue; };
  wait_till_pid(pid, 3);
}
