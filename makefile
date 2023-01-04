all: thr_atomic thr_reduce

thr_atomic: thr_atomic.o
	gcc -o thr_atomic thr_atomic.o -lm -lpthread

thr_atomic.o: thr_atomic.c
	gcc -c thr_atomic.c

thr_reduce: thr_reduce.o
	gcc -o thr_reduce thr_reduce.o -lm -lpthread

thr_reduce.o: thr_reduce.c
	gcc -c thr_reduce.c

clean:
	rm *.o thr_atomic thr_reduce