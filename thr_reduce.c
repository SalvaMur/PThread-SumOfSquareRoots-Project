#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int m, n;           //"m" is the number of threads, "n" is the end-range for summing square roots.
double* sum;        //Double pointer is used for gathering local sums from threads.
pthread_t* thrP;    //Pointer for accessing status of threads while inside other threads.

int powerOfTwoCheck(int t) {        //Function explained in thr_atomic.c.
    while (t%2 == 0 && t != 0) {
        t = t/2;
    }

    if (t%2 == 1 && t != 1) {
        return 1;
    }

    else {
        return 0;
    }
}

void* calc(void* arg) {
    long tid = (long) arg;      //Thread ID is the index passed in by the for-loop in main.
    double lsum = 0;            //Initializes local sum to 0.
    for (int i = tid*n/m + 1; i <= (tid + 1)*n/m; i++) {        //Does calculation exactly as in thr_atomic.c.
        lsum += pow((double) i, 0.5);
    }

    sum[tid] = lsum;                        //Adds thread's local sum to its corresponding index in the global "sum" pointer/array.
    for (long d = m/2; d >= 1; d /= 2) {    //For-loop that determines thread partner by using the distance "d". Keeps dividing "d" by 2 as long as thread has a partner to be joined by.
        if (tid < (m - d)) {                //Checks if thread does indeed have a partner to be joined by. 
            //printf("Thread %ld has no parter to be joined to.\n", tid);
            pthread_exit(NULL);                                         //If thread does not have a partner to be joined by, thread will finish.
        }

        else {                                      //If thread does have a joinable partner, it will wait for it to finish by calling pthread_join(...).
            pthread_join(thrP[tid - d], NULL);      //"thrP" is used as using "thr" causes an error, since it is undeclared within the thread's scope.
            sum[tid] += sum[tid - d];               //Once the partner thread finishes, it adds its partner's corresponding value within the "sum" pointer array to its own.
            //printf("Thread %ld joined Thread %ld.\tCombined sum of threads is %f.\n", tid - d, tid, sum[tid]);
        }
    }

    if (tid == (m - 1)) {       //Once for-loop finishes, an if-statement checks if the thread ID is the last thread created.
        printf("Sum = %f\n", sum[tid]);        //If it is the last created thread, it will print its corresponding values in the "sum" pointer array which will be the sum of square roots up to the end-range "n".
    }
}

int main(int argc, char* argv[]) {      //If-statements and assigning values for "m" and "n" explained in thr_atomic.c.
    if (argc < 3) {
        printf("Please provide valid commands.\n");
        exit(0);
    }

    m = atoi(argv[1]), n = atoi(argv[2]);
    printf("m = %d n = %d\n", m, n);
    
    if (m == 0 || n == 0) {
        printf("Please do not enter 0 for \"m\" or \"n\".\n");
        exit(0);
    }

    if (powerOfTwoCheck(m) == 1) {
        printf("Please input a power of 2 for \"m\".\n%d is not a power of 2.\n", m);
        exit(0);
    }

    if (powerOfTwoCheck(n) == 1) {
        printf("Please input a power of 2 for \"n\".\n%d is not a power of 2.\n", n);
        exit(0);
    }

    sum = malloc(m*sizeof(double));     //Allocates memory for global pointer "sum" that corresponds to the number of threads.
    thrP = malloc(m*sizeof(pthread_t)); //Allocates memory for global thread pointer that corresponds to the number of threads.
    long i;                             //Used as an index for the for-loop and as an ID for threads.
    pthread_t thr[m];                   //Creats "m" amount of threads.
    thrP = thr;                         //Copies memory addresses of "thr" in order to point to threads.
    for (i = 0; i < m; i++) {       //Explained in thr_atomic.c.
        if (pthread_create(&thr[i], NULL, calc, (void*) i) != 0) {
            printf("Cannot create a thread.\n");
            exit(1);
        }
    }

    pthread_exit(NULL);      //Waits for all threads to finish, then exits.
}