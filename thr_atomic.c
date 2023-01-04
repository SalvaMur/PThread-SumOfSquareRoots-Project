#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int m, n;       //"m" is the number of threads, "n" is the end-range for summing square roots.
double sum = 0; //Global variable that accumulates the local sums from each thread.
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //

int powerOfTwoCheck(int t) {        //Function used to check whether "m" and "n" are powers of 2.
    while (t%2 == 0 && t != 0) {    //Loop divides "t" by 2 as long as the remainder is 0.
        t = t/2;
    }

    if (t%2 == 1 && t != 1) {       //After while-loop finishes, "t" will either be 1 if it is a power of 2 or it will be a non-power of 2.
        return 1;                   //If it's a non-power of 2, 1 will be returned. If is a power of 2, it will return 0.
    }

    else {
        return 0;
    }
}

void* calc(void* arg) {         //Function that is invoked by all child threads in line 61 by pthread_create(...).
    long tid = (long) arg;      //The thread ID is assigned by taking the index passed in by the for-loop in lines 60-61.
    double lsum = 0;            //The local sum that is the total sum of square roots within a thread.
    for (int i = tid*n/m + 1; i <= (tid + 1)*n/m; i++) {        //Starting index of for-loop is the start of the range for a thread. For-loop will only go up to the end range of the thread.
        lsum += pow((double) i, 0.5);       //Local sum accumulates all square roots within the thread's range.
    }

    printf("Start: %ld\tEnd: %ld, Thread %ld: %f\n", tid*n/m+1, (tid+1)*n/m, tid, lsum);

    pthread_mutex_lock(&lock);
    sum += lsum;        //At the end of the function, every thread adds their local sum to the global sum.
    pthread_mutex_unlock(&lock);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {     //Checks if there are not enough arguments.
        printf("Please provide valid commands.\n");
        exit(0);
    }

    m = atoi(argv[1]), n = atoi(argv[2]);       //Gives "m" and "n" their respective values with atoi().
    printf("m = %d n = %d\n", m, n);
    
    if (m == 0 || n == 0) {     //Checks if "m" or "n" are 0.
        printf("Please do not enter 0 for \"m\" or \"n\".\n");
        exit(0);
    }

    if (powerOfTwoCheck(m) == 1) {      //Checks if "m" is a power of 2.
        printf("Please input a power of 2 for \"m\".\n%d is not a power of 2.\n", m);
        exit(0);
    }

    if (powerOfTwoCheck(n) == 1) {      //Checks if "n" is a power of 2.
        printf("Please input a power of 2 for \"n\".\n%d is not a power of 2.\n", n);
        exit(0);
    }

    long i;     //Used as an index for the for-loops and as an assinged ID number for threads.
    
    pthread_t thr[m];       //Creating "m" amount of threads.
    for (i = 0; i < m; i++) {       //For-loop that begins the process for each thread.
        if (pthread_create(&thr[i], NULL, calc, (void*) i) != 0) {      //pthread_create(...) will not return 0 if something bad happens.
            printf("Cannot create a thread.\n");                        //We use this fact to make main exit if an error happens during thread creation.
            exit(1);
        }
    }

    for (i = 0; i < m; i++) {       //Waits for each individual thread to finish by calling pthread_join().
        pthread_join(thr[i], NULL); //If this is not done, the main thread will finish faster than the child threads thus prematurely exiting.
    }

    printf("Sum = %f\n", sum);      //Once all threads have finished in the above for-loop, the global sum is printed.
}