#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

typedef long long int ll; 
float *burst_time;

int main(int argc, char *argv[]){
    
    if (argc != 4) {
		printf("Usage: ./P3_final.out <thread numbers> <num of ints> <inputfile_name>\n");
		exit(-1);
	}

    char* fname = argv[3]; 
	int thread_number = atoi(argv[1]);
    int num_int = atoi(argv[2]); 

    //quantum time
    double qt = 0.001;

    int memIDbt;
    int memSizebt = sizeof(double)*(2);
    key_t keybt = (key_t)1234;
    memIDbt = shmget(keybt, memSizebt, 0666|IPC_CREAT);

    burst_time = shmat(memIDbt, (void*)0, 0);
    
    burst_time[2] =0;
    burst_time[3] =0;

    pid_t child_a, child_b, _;
 
    child_a = fork();
    if (child_a == 0){    
        execlp("./P1.out","./P1.out", argv[1], argv[2], argv[3],NULL);
        
    }
    else{
        child_b = fork();
        if (child_b == 0){
		    execlp("./P2.out","./P2.out", argv[1],NULL);
        }
        else{
        	kill(child_b, SIGSTOP);
        	kill(child_a, SIGSTOP);
            
            int flag = 0, status = 0;
            printf("inside P3\n");
			
			ll bt1=0;
			ll bt2=0; 
			
			clock_t start_p1;
    		clock_t start_p2;
    		clock_t start_p3 = clock();

            double tat1, tat2;

            while(!(int)burst_time[2] && !(int)burst_time[3]){
                if(!flag){
                    //kill(child_b, SIGSTOP);
                    start_p1 = clock();
                    kill(child_a, SIGCONT);
                    sleep(qt);
                    kill(child_a, SIGSTOP);
                    bt1+=clock()-start_p1;
                    flag = 1;
                }
                else{
                    //kill(child_a, SIGSTOP);
                    start_p2 = clock();
                    kill(child_b, SIGCONT);
                    sleep(qt);
                    kill(child_b, SIGSTOP);
                    bt2+=clock()-start_p2;
                    flag = 0;
                }
            }
            if(burst_time[2]){
                tat1 = (double)(clock()-burst_time[0]) / (double)CLOCKS_PER_SEC;
                while(!burst_time[3]){
                    start_p2 = clock();
                    kill(child_b, SIGCONT);
                    sleep(qt);
                    kill(child_b, SIGSTOP);
                    bt2+=clock()-start_p2;
                }
                tat2 = (double)(clock()-burst_time[1]) / (double)CLOCKS_PER_SEC;
            }
            else{
                tat2 = (double)(clock()-burst_time[1]) / (double)CLOCKS_PER_SEC;
                while(!burst_time[2]){	
                    start_p1 = clock();
                    kill(child_a, SIGCONT);
                    sleep(qt);
                    kill(child_a, SIGSTOP);
                    bt1+=clock()-start_p1;
                }
                tat1 = (double)(clock()-burst_time[0]) / (double)CLOCKS_PER_SEC;
            }
			
            clock_t end_p3 = clock();

            double bt_1 = (double)bt1 / (double)CLOCKS_PER_SEC;
            double bt_2 = (double)bt2 / (double)CLOCKS_PER_SEC;
            
            if(tat2>qt) tat2-=qt;
            
            printf("Burst Time P1 = %lf, Burst Time P2 = %lf\n", bt_1, bt_2);


            double wt1, wt2;
            if(bt_1<qt){
            	tat1 = bt_1;
                wt1 = 0;
                wt2 = tat2-bt_2;
            }
            else{
                wt1 = tat1-bt_1;
                wt2 = tat2-bt_2;
            }
            printf("Turnaround time of Process 1 = %lf \n", tat1);
            printf("Turnaround time of Process 2 = %lf \n", tat2);
            printf("Waiting time of Process 1 = %lf \n", wt1);
            printf("Waiting time of Process 2 = %lf \n", wt2);
        }
    } 
}