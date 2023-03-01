#include <iostream> //including the libraries 
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include<unistd.h>
#include<string>
using namespace std;
//declaring global variables 
#define NUM_THREADS 8 // constant NUM_THREADS represents the number of threads to be created
#define MAX 1000000
pthread_mutex_t mutx; //declares a variable that is used to synchronize access to a shared variable between multiple threads
//data array with max size
int data[MAX];
long global_sum = 0; //intialising the value of global_sum varible
//each thread will execute. This function receives a parameter threadid, which is a unique identifier for the thread. 
//Inside the function, the thread computes the sum of a portion of the array based on its threadid. The computation is done in parallel by multiple threads.
void *sum(void *threadid) //receives a parameter threadid, which is a unique identifier for the thread
{
   long tid; // thread computes the sum of a portion of the array based on its threadid. The computation is done in parallel by multiple threads.
   long sum = 0;
   tid = (long)threadid;
  // printf("Bye World! It's me, thread #%ld!\n", tid);
  int range = MAX/NUM_THREADS;
  int start = tid * range ;
  int end = start + range ;

  for(int i = start ; i < end ; i++) {
        sum += data[i];
  }

  //enforce sequential update
  pthread_mutex_lock(&mutx);// updates the global sum variable with its local sum. A mutex is used to synchronize access to the global_sum variable
  // to prevent data races, where multiple threads simultaneously update the same variable
  global_sum += sum;

  pthread_mutex_unlock(&mutx);

  //enforce sequential update

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{ 
    int cores = std::thread::hardware_concurrency(); // first initializes a mutex using pthread_mutex_init. It then determines the number of cores 
    //on the machine using std::thread::hardware_concurrency. This value is used to divide the array into equal portions for each thread to compute.
    pthread_mutex_init(&mutx, NULL);

    cout<<" the number of cores on this machine = "<<cores<<endl;
   // srand(time(NULL));
   for(int i = 0; i < MAX; i++) {
       data[i] = rand() % 20;
   }

   pthread_t threads[NUM_THREADS];//rogram then creates NUM_THREADS threads using pthread_create, passing the sum function and a unique threadid 
   //as arguments. It then waits for all threads to complete their computation using pthread_join.
   long tid = 0;

   for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_create(&threads[tid], NULL, sum, (void *)tid);
   }


   for(tid = 0; tid < NUM_THREADS; tid++) {
       pthread_join(threads[tid], NULL);
   }

   
   cout<<"the final sum ="<<global_sum<<endl;//prints the final value of global_sum
}