#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define max_elem 1000000

typedef long long int ll; 

typedef struct{
	int start;
    // seg_size = end - start + 1
    int seg_size;
} sum_arg;

int *shared;

void * thread_sum(void * arg){
    sum_arg* temp = (sum_arg*) arg;

    ll* sum = malloc(sizeof(ll));
    *sum = 0;
    for(int i = temp->start; i < temp->start + temp->seg_size; i++){
		*sum+=shared[i+1];
    }
    return (void*)sum;
}

void make_thread(int i, int t, int start, int seg_size, sum_arg *arg, pthread_t *threads){
    arg[i].start = start;
    arg[i].seg_size = seg_size;

    pthread_create(&threads[i], NULL, &thread_sum, (void*) &arg[i]);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
		printf("Usage: ./P2_final.out <thread numbers>\n");
		exit(-1);
	}
	
    int thread_number = atoi(argv[1]);

    ll sums_arr[max_elem] = {0};
    
    int memID;
    int memSize = sizeof(int)*(1000000 + 1);
    
    key_t key = (key_t)69420;
    memID = shmget(key, memSize, 0666|IPC_CREAT);

    shared = shmat(memID, (void*)0, 0);

    int num_int = shared[0];

    clock_t start_time = clock();

    sum_arg arg[thread_number];
    pthread_t threads[thread_number];

    int start=0;
    int seg_size = num_int/thread_number;

    // Creating Threads
    for(int i = 0; i<thread_number; i++){
        if(i==thread_number-1){
            seg_size += num_int%thread_number;
        }
        make_thread(i, thread_number, start, seg_size, arg, threads);
        start += seg_size;
    }

    ll* result;
    ll sum = 0;
    // Joining threads
    for(int i = 0; i<thread_number; i++){
        pthread_join(threads[i], (void**) &result);
        sums_arr[i] = *result;
        free(result);
    }

    for(int i = 0; i<thread_number; i++){
            sum+=sums_arr[i];
        }
    clock_t stop_time = clock();
    double elapsed_time = (double)(stop_time - start_time) / (double)CLOCKS_PER_SEC;
    
    // storing logs in a txt file
    // FILE *fp;
    // fp = fopen("log_p2.txt", "w");
    // if(fp == NULL)
    //     exit(-1);
    // fprintf(fp, "%d %d %lf %lld\n", num_int, thread_number, elapsed_time, sum);
    // fclose(fp);
    return 0;
}