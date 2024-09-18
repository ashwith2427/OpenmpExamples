#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#define N 80000000

void cpu_timer_start(struct timespec* tStart){
	clock_gettime(CLOCK_MONOTONIC, tStart);
}
double cpu_timer_stop(struct timespec* tStart){
	struct timespec tend;
	clock_gettime(CLOCK_MONOTONIC, &tend);

	double start_sec = (double)tStart->tv_sec + (double)tStart->tv_nsec/1e9;
	double end_sec = (double)tend.tv_sec + (double)tend.tv_nsec/1e9;
	return end_sec - start_sec;
}

void merge_sort(int* A, int size, int* temp, int threads);
void merge_sort_serial(int* A,int size, int* temp);
void merge_sort_helper(int* A, int size, int* temp);
void merge(int* A, int size, int* temp);

int main(){
    srand(314159);
    int* A = (int*)malloc(N*sizeof(int));
    int* temp = (int*)malloc(N*sizeof(int));
    #pragma for parallel simd
    for(int i=0;i<N;i++){
        A[i] = (rand()%N)+1;
        //A[i] = N - i;
    }
    struct timespec tStart;
    omp_set_nested(1);
    cpu_timer_start(&tStart);
    merge_sort(A, N, temp, omp_get_max_threads());
    double parallel_time = cpu_timer_stop(&tStart);
    cpu_timer_start(&tStart);
    merge_sort_serial(A, N, temp);
    double sequential_time = cpu_timer_stop(&tStart);

    // for(int i=0;i<N;i++){
    //     printf("%d ",A[i]);
    // }
    // printf("\n");
    printf("Elapsed time:\n");
    printf("Using parallel approach: %lf\n",parallel_time);
    printf("Using sequential approach: %lf\n",sequential_time);
    
    return 0;
}

void merge_sort(int* A,int size, int* temp, int threads){
    if(threads <= 1){
        merge_sort_helper(A,size,temp);
        return;
    }else{

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                merge_sort(A,size/2,temp,threads/2);
            }
            #pragma omp section
            {
                merge_sort(A+size/2, size-size/2, temp+size/2,threads-threads/2);
            }
        }
        merge(A,size,temp);
    }
}

void merge_sort_serial(int* A, int size, int* temp){
    if(size <= 1) return;
    merge_sort_serial(A, size/2, temp);
    merge_sort_serial(A + size/2, size-size/2, temp);
    merge(A, size, temp);
}
void merge_sort_helper(int* A, int size, int* temp){
    if(size <= 1) return;
    #pragma omp task
    merge_sort_serial(A, size/2, temp);
    #pragma omp task
    merge_sort_serial(A + size/2, size-size/2, temp);
    merge(A, size, temp);
}

void merge(int* A, int size, int* temp){
    int i = 0;
    int mid = size/2;
    int j = mid;
    int k = 0;

    while(i<mid && j<size){
        if(A[i]<=A[j]){
            temp[k] = A[i];
            i++;
        }else{
            temp[k] = A[j];
            j++;
        }
        k++;
    }

    while(i<mid){
        temp[k] = A[i];
        i++;
        k++;
    }

    while(j<size){
        temp[k] = A[j];
        j++;
        k++;
    }

    memcpy(A, temp, size* sizeof(int));
}