# PThread Project

## Description

Two programs that both calculate the sum of square roots from one to the user's input, which should be a power of two. The two programs would also take in the number of threads to use based on what the user enters. The file 'thr_atomic.c' atomically sums all the sums calculated by each thread. The file 'thr_reduce.c' instead makes threads wait on a designated partner thread to which the partner thread's calculated sum will be added to the current thread (the one that waited for its partner).

File 'thr_atomic.c' uses a global variable, 'sum', to add all the thread's calculations. Because of race conditions (two different threads in sync adding their local sum to the global sum at the same time), the critical region (thread adding towards global sum) is protected with a mutex lock to prevent such an event.

File 'thr_reduce.c' does not use a global variable, nor does it need a mutex lock as the code utilizes 'pthread_join()' to accumulate the sums across each thread. Each thread waits on its own partner to finish once the waiting thread itself finishes calculating its range of square roots. Threads will continue waiting for their partner until the last pair of threads finally join. The last thread in the process will have the final sum of square roots, which it obtained from all joined threads.