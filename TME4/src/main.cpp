#include "Banque.h"
#include <chrono>
#include <thread>
#include <iostream>
#define NB_ACCOUNT 5
using namespace std;
void work(pr::Banque& b) {
	int i, j, m, s;
	for (size_t x = 0; x < 1000; ++x) {
		i = rand() % NB_ACCOUNT;
		j = rand() % NB_ACCOUNT;
		m = rand() % 100;
		//s = rand() % 20 + 1;
		b.transfert(i, j, m);
		//std::this_thread::sleep_for(std::chrono::milliseconds(s));
	}
}
void workcomptable(pr::Banque& b,int iteration) {
	for (int i = 0; i < iteration; ++i) {
  		if (b.comptabiliser(NB_ACCOUNT * 200)==1)
    		std::cout << "OK" << std::endl;
		else std::cout << "KO" << std::endl;
	}
}
const int NB_THREAD = 10;
int main () {
	vector<thread> threads;
	pr::Banque b(NB_ACCOUNT, 200);
	// TODO : creer des threads qui font ce qui est demandé
	for (int a = 0; a < NB_THREAD; ++a) //on crée les 10 threads
		threads.emplace_back(work, std::ref(b));
	threads.emplace_back(workcomptable, std::ref(b),10); //thread comptable
	for (auto & t : threads)
		t.join();
	return 0;
}
