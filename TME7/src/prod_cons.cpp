#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

std::vector<pid_t> conso;

void handler(int signal){
	for(auto c : conso){
		kill(c,SIGINT);
	}
}

int main () {

	void * addr = mmap(0, sizeof(Stack<char>), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);

	/*si pas anonyme*/
	/*

	 int fd = shm_open("/myshm", O_CREAT|O_EXCL|O_RDWR, 0600);
	 ftruncate(fd, sizeof(Stack<char>));

	 void * addr = mmap(0, sizeof(Stack<char>), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	 */
	Stack<char> * s = new (addr) Stack<char>();



	pid_t pp = fork();
	if (pp==0) {
		producteur(s);
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}
	else{
		conso.push_back(pc);
	}

	signal(SIGINT, handler);

	wait(0);
	wait(0);

	s->~Stack<char>();
	munmap(addr, sizeof(Stack<char>));

	/*si pas anonyme*/
	/*
	 shm_unlink("/myshm");
	 */

	return 0;
}

