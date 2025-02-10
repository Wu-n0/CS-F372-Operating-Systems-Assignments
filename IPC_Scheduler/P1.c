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

typedef long long int ll; 

typedef struct{
	int start;
    // seg_size = end - start + 1
    int seg_size;
	ll seg_start;
    char* fname;
} segment;

int *shared;

void * read_file(void * seg){
    segment* temp = (segment*) seg;
    FILE* file = fopen(temp->fname, "r");
    fseek(file , temp->seg_start, SEEK_SET);
    for(int i = temp->start; i < temp->start + temp->seg_size; i++)
    {
		fscanf(file, "%d", &shared[i+1]);
    }
}

int make_thread(int i, int t, int start, int seg_size, ll seg_start, char* fname, segment *seg, pthread_t *threads){
    seg[i].start = start;
    seg[i].seg_size = seg_size;
    seg[i].seg_start = seg_start;
    seg[i].fname = fname;

    FILE* ip_file = fopen(fname,"r");
	
    pthread_create(&threads[i], NULL, &read_file, (void*) &seg[i]);

    fseek(ip_file, seg_start, SEEK_SET);
	int count=0;
	if(i<t-1){
        while(1){
            if(getc(ip_file)==32){
                count++;
            }
            if(count == seg_size){
                seg_start = ftell(ip_file);
                break;
            }
        }
    }
	
    fclose(ip_file);

    return seg_start;
}

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		printf("Usage: ./P1_final.out <num of ints> <inputfile_name> <thread numbers>\n");
		exit(-1);
	}
	// ./P1_final.out 100 hundred.txt 1
    char* fname = argv[2]; 
	int thread_number = atoi(argv[3]);
    int num_int = atoi(argv[1]);

    int memID;
    int memSize = sizeof(int)*(1000000 + 1);
    key_t key = (key_t)69420;
    memID = shmget(key, memSize, 0666|IPC_CREAT);

    shared = shmat(memID, (void*)0, 0);
    shared[0] = num_int;
    memset(shared+1, 0, sizeof(int)*(1000000));

    //Start clk time
    clock_t start_time = clock();

    segment seg[thread_number];
    pthread_t threads[thread_number];

    ll seg_start = 0;
    int start=0;
    int seg_size = num_int/thread_number;

    // Creating Threads
    for(int i = 0; i<thread_number; i++){
        if(i==thread_number-1){
            seg_size += num_int%thread_number;
        }
        seg_start = make_thread(i, thread_number, start, seg_size, seg_start, fname, seg, threads);
        start += seg_size;
    }

    // Joining threads
    for(int i = 0; i<thread_number; i++) pthread_join(threads[i], NULL);

    clock_t stop_time = clock();
    double elapsed_time = (double)(stop_time - start_time) / (double)CLOCKS_PER_SEC;

    // storing logs in a txt file
    // FILE *fp;
    // fp = fopen("log_p1.txt", "w");
    // if(fp == NULL)
    //     exit(-1);
    // fprintf(fp, "%d %d %lf\n", num_int, thread_number, elapsed_time);
    // fclose(fp);
    return 0;
}