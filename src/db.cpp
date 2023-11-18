#include <thread>
#include "./data.hpp"

constexpr int REPETITION = 10000;
constexpr int DATA_SIZE = 100000;
DataList dl(DATA_SIZE);

void worker(){
    for (int k = 0; k < REPETITION; k++) {
        if (k%500 == 0){
           // std::clog<<"Worker "<<std::this_thread::get_id()<<" finished "<<k<<" iterations"<<std::endl;
        }
        int j = rand() % (DATA_SIZE+1);
        int i = rand() % (DATA_SIZE+1);
        dl.set_j_with_i(i, j);
    }

}
int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Arguments needed: num_workers" << std::endl;
        exit(1);
    }
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    int num_workers = atoi(argv[1]);
    std::thread workers[num_workers];
    for (int i = 0; i < num_workers; i++) {
        workers[i] = std::thread(worker);
    }
    for(auto& t : workers){
        t.join();
    }
    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end-start;
    std::clog<<"Time elapsed: "<<diff.count()<<" ms"<<std::endl;
}