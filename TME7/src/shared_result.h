#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#include <cstring> // size_t,memset

namespace pr {

#define STACKSIZE 100

template<typename T>
class SharedResult{
	T res;
	sem_t sem;

public:
	SharedResult(){
		sem_init(&sem, 1, 0);
	}
	~SharedResult(){
		sem_destroy(&sem);
	}

	void set_result(const T& r){
		res = r;
		sem_post(&sem);
	}

	T* get_result(){
		sem_wait(&sem);
		return &res;
	}


};

int main(){
	void * addr = mmap(0, sizeof(SharedResult<char>), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	SharedResult<char> * sr = new (addr) SharedResult<char>();

	pid_t fils;
	char *res;

	if((fils = fork() == -1)){
		perror("fork");
		exit(2);
	}
	if(fils == 0){
		char r = "A";
		sr->set_result(r);
		exit(0);
	}

	if((fils = fork() == -1)){
		perror("fork");
		exit(2);
	}
	if(fils == 0){
		res = sr->get_result();
		exit(0);
	}

	wait(nullptr);
	wait(nullptr);
	sr->~SharedResult();
	munmap(addr, sizeof(SharedResult<char>));
}
