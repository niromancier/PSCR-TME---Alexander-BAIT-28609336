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
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t sem_push;
	sem_t sem_pop;
	sem_t sem_mutex;

public :
	Stack () : sz(0) {
		memset(tab,0,sizeof tab);
		sem_init(&sem_push, 1, STACKSIZE);
		sem_init(&sem_pop, 1, 0);
		sem_init(&sem_mutex, 1, 1);
	}

	~Stack(){
		sem_destroy(&sem_push);
		sem_destroy(&sem_pop);
		sem_destroy(&sem_mutex);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&sem_pop);
		sem_wait(&sem_mutex);
		T toret = tab[--sz];
		sem_post(&sem_mutex);
		sem_post(&sem_push);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&sem_push);
		sem_wait(&sem_mutex);
		tab[sz++] = elt;
		sem_post(&sem_mutex);
		sem_post(&sem_pop);
	}
};

}
