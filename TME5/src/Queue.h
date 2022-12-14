#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstdlib>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <cstring>

namespace pr {

// MT safe version of the Queue, non blocking.
template <typename T>
class Queue {
	T ** tab;
	const size_t allocsize;
	size_t begin;
	size_t sz;
	mutable std::mutex m;
	std::condition_variable cond_prod, cond_cons; //ajout des cond
	bool block; //ajout du booléen

	// fonctions private, sans protection mutex
	bool empty() const {
		return sz == 0;
	}
	bool full() const {
		return sz == allocsize;
	}
public:
	Queue(size_t size) :allocsize(size), begin(0), sz(0) {
		tab = new T*[size];
		memset(tab, 0, size * sizeof(T*));
		block=true;//initialisation du bool
	}
	size_t size() const {
		std::unique_lock<std::mutex> lg(m);
		return sz;
	}
	T* pop() {
		std::unique_lock<std::mutex> lg(m);
		while (empty() && block) cond_cons.wait(lg); //bloquant si vide et bloquant sur la consommation
    	if (empty() && !block) return nullptr; //rend null si vide et non bloquant
    	cond_prod.notify_one(); //sinon on continue et notify la production
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin + 1) % allocsize;
		return ret;
	}
	bool push(T* elt) {
		std::unique_lock<std::mutex> lg(m);
		while (full() && block) cond_prod.wait(lg); //si plein et bloquant sur la production
		if (full() && !block) return false; //rend false si plein et non bloquant
		cond_cons.notify_one(); //sinon on continue et notify la consommation
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}
	~Queue() {
		// ?? lock a priori inutile, ne pas detruire si on travaille encore avec
		std::unique_lock<std::mutex> lg(m);
		for (size_t i = 0; i < sz; i++) {
			auto ind = (begin + i) % allocsize;
			delete tab[ind];
		}
		delete[] tab;
	}
	void setBlocking(bool b) { //ajout de la fonction setBlocking
	{
		std::unique_lock<std::mutex> lg(m);
		block=b;
	}
	cond_cons.notify_all(); //notify la consommation de l'état du block
	}
};
}
#endif /* SRC_QUEUE_H_ */
