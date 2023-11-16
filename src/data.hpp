#include <iostream>
#include <vector>
#include <mutex>
#include <shared_mutex>

class DataList{
public:
    DataList(int size): dl_size(size){
        assert(size >= 100);
        // Initialize to all 1s
        data_list = std::vector<u_int>(size,1);
        // 100 consecutive elements share a lock
        locks = std::vector<std::shared_mutex>(size/100+1);
        std::clog<<"DataList initialized"<<std::endl;
    }
    void set_j_with_i(int i, int j){
        int i_1 = (i+1)%dl_size;
        int i_2 = (i+2)%dl_size;
        int lock_i = i/100;
        int lock_i_2 = i_2/100;
        int lock_j = j/100;
        std::unique_lock<std::shared_mutex> w_lock(locks[lock_j],std::defer_lock);

        if(lock_j != lock_i && lock_j != lock_i_2){
            // lock of j is not that of i~i+2
            if (lock_i!=lock_i_2){
                std::shared_lock<std::shared_mutex> r_lock_0(locks[lock_i]);
                std::shared_lock<std::shared_mutex> r_lock_2(locks[lock_i_2]);
                w_lock.lock();
                data_list[j] = data_list[i_1]+data_list[i_2]+data_list[i];
            }else{
                std::shared_lock<std::shared_mutex> r_lock_0(locks[lock_i]);
                w_lock.lock();
                data_list[j] = data_list[i_1]+data_list[i_2]+data_list[i];
            }
        }else if (lock_j != lock_i_2){
            // l_j == l_i
            std::shared_lock<std::shared_mutex> r_lock_2(locks[lock_i_2]);
            w_lock.lock();
            data_list[j] = data_list[i_1]+data_list[i_2]+data_list[i];
        }else if (lock_j != lock_i) {
            // l_j == l_i_2
            std::shared_lock <std::shared_mutex> r_lock_0(locks[lock_i]);
            w_lock.lock();
            data_list[j] = data_list[i_1] + data_list[i_2] + data_list[i];
        }else{
            // l_j == l_i == l_i_2
            w_lock.lock();
            data_list[j] = data_list[i_1]+data_list[i_2]+data_list[i];
        }
    }

private:
    u_int dl_size;
    std::vector<u_int> data_list;
    std::vector<std::shared_mutex> locks;
};
