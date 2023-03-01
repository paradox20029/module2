#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <ctime>

#define NUM_THREADS 8
#define MAX 1000000

std::mutex sum_mutex; // declares a variable that is used to synchronize access to a shared variable between multiple threads
int data[MAX];
long global_sum = 0;

void sum(int tid)
{
   long sum = 0;
  
   // calculates the range of indices in the data array that each thread will process, based on the total number of threads and the size of the array
   int range = MAX/NUM_THREADS;
   int start = tid * range ;
   int end = start + range ;

   // iterates over a portion of the data array and computes the sum of the integers in that portion
   for(int i = start ; i < end ; i++) {
        sum += data[i];
   }

   // locks the mutex variable to ensure that only one thread at a time can access the shared variable that stores the sum of the array
   std::lock_guard<std::mutex> guard(sum_mutex);
   global_sum += sum;
}

int main(int argc, char *argv[])
{
    int cores = std::thread::hardware_concurrency();

    std::cout << "total number of cores on this machine = " << cores << std::endl;

   // fills the data array with random integers
   std::srand(std::time(nullptr));
   for(int i = 0; i < MAX; i++) {
       data[i] = std::rand() % 20;
   }

   // creating a vector of threads and assigns each thread a portion of the array to be processed
   std::vector<std::thread> threads;
   for(int tid = 0; tid < NUM_THREADS; tid++) {
       threads.emplace_back(sum, tid);
   }

   // waits for all the threads to finish executing before proceeding
   for(auto& thread : threads) {
       thread.join();
   }

   // prints out the final sum of the array
   std::cout << "final sum = " << global_sum << std::endl;

   return 0;
}