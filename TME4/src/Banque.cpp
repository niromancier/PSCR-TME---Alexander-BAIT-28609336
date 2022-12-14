#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	//unique_lock<mutex> g(m);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	lock(debiteur.getMutex(), crediteur.getMutex()); // lock the mutexs
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	debiteur.getMutex().unlock(); // unlock the mutexs
	crediteur.getMutex().unlock();
}
size_t Banque::size() const {
	//unique_lock<mutex> g(m);
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	//unique_lock<mutex> g(m);
	int bilan = 0;
	int id = 0;
	for (auto & compte : comptes) {
		compte.getMutex().lock(); // lock the mutexs
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	for (auto & compte : comptes)
		compte.getMutex().unlock(); // unlock the mutexs
	return bilan == attendu;
}
//std::mutex & Banque::getMutex() {
	//return m;
//}
}
