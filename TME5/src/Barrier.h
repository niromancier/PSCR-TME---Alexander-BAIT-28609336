#pragma once
#include <mutex>
#include <condition_variable>

namespace pr {

class Barrier {
	std::mutex m;
	int cpt;
	int n;
	std::condition_variable cv;

public:

	Barrier(int n) : cpt(0), n(n){}

	void done(){
		std::unique_lock<std::mutex> l(m);

		++cpt;

		if(cpt == n){
			cv.notify_all();
		}
	}

	void waitFor(){
		std::unique_lock<std::mutex> l(m);
		while(cpt < n){
			cv.wait(l);
		}
	}


};
}