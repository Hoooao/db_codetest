## Code Test

### 使用方法：


```
git clone https://github.com/Hoooao/db_codetest.git
cd ./db_codetest/src
make
./db <num_workers>
```


### 设计
worker通过获取对应的W/R锁来对对应数据进行取用和修改。由于数据量大，元素层次上的锁会造成过大的内存开销，所以采用了分段锁的方式：连续的`LOCK_SEG_SIZE`个数据共享同一个锁。

### 死锁

死锁是可能出现的，参考如下例子：
 
    假设两个事务分别获取对应的锁：
    事务1：获取read-lock:A, write-lock: B
    事务2：获取read-lock:B, write-lock: A
    当T1获取了A的read-lock，T2获取了B的read-lock，此时T1想要获取B的write-lock，T2想要获取A的write-lock，就会发生死锁。

**避免方式**：

    1. 通过更小的锁粒度，减小死锁发生概率；
    2. 事务在获取锁的时候，按照一定的顺序获取，比如按照锁的名称排序；
    3. 用`try_lock_for`不断尝试获取锁；✅
    