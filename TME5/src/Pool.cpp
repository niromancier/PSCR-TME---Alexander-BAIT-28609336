#include "Pool.h"
namespace pr {
    void poolJob(Queue<Job>& queue) { //job pour les threads
        while (true) {
            Job* j = queue.pop();
            if (j == nullptr) break;
            j->run();
            delete j;
        }
    }
    Pool::Pool(int qsize) : queue(qsize) {} //creer une queue
    void Pool::submit(Job* job) { queue.push(job); } //submit un job
    void Pool::start(int nbthread) { //commencer les threads
        for (int i = 0; i < nbthread; ++i)
            threads.emplace_back(poolJob, std::ref(queue));
    }
    void Pool::stop() { //stopper les threads
        queue.setBlocking(false);
        for (auto& t : threads) 
            t.join();
    }
    Pool::~Pool() {delete &queue;} //supprimer la queue
}