#include <iostream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <cassert>

// LOCK_SEG_SIZE has to be greater than 1
constexpr int LOCK_SEG_SIZE = 50;
class DataList{
public:
    DataList(int size): dl_size(size){
        assert(LOCK_SEG_SIZE > 1);
        assert(size >= LOCK_SEG_SIZE);
        // Initialize to all 1s
        data_list = std::vector<u_int>(size,1);
        // 100 consecutive elements share a lock
        locks = std::vector<std::shared_timed_mutex>(size/LOCK_SEG_SIZE+1);
        // std::clog<<"DataList initialized"<<std::endl;
    }
    void set_j_with_i(int i, int j){

        int i_1 = (i+1)%dl_size;
        int i_2 = (i+2)%dl_size;
        int lock_i = i/LOCK_SEG_SIZE;
        int lock_i_2 = i_2/LOCK_SEG_SIZE;
        int lock_j = j/LOCK_SEG_SIZE;

        while (true){

            // Process j overlaps with i~i+2
            std::shared_lock<std::shared_timed_mutex> r_lock_0(locks[lock_i], std::defer_lock);
            std::shared_lock<std::shared_timed_mutex> r_lock_2(locks[lock_i_2], std::defer_lock);
            if(lock_j != lock_i && lock_j != lock_i_2){
                // lock of j is not those of i~i+2
                if (lock_i!=lock_i_2){
                    if (!r_lock_2.try_lock_for(std::chrono::milliseconds(2))) {
                        continue;
                    }
                }
                if (!r_lock_0.try_lock_for(std::chrono::milliseconds(2))) {
                    continue;
                }
            }else if (lock_j != lock_i_2){
                // l_j == l_i
                if (!r_lock_2.try_lock_for(std::chrono::milliseconds(2))) {
                    continue;
                }
            }else if (lock_j != lock_i) {
                // l_j == l_i_2
                if (!r_lock_0.try_lock_for(std::chrono::milliseconds(2))) {
                    continue;
                }
            }
            std::unique_lock<std::shared_timed_mutex> w_lock(locks[lock_j], std::defer_lock);
            if (!w_lock.try_lock_for(std::chrono::milliseconds(2))) {
                continue;
            }
            data_list[j] = data_list[i_1]+data_list[i_2]+data_list[i];
            break;
        }
    }

private:
    u_int dl_size;
    std::vector<u_int> data_list;
    std::vector<std::shared_timed_mutex> locks;
};
