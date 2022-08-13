#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <atomic>
#include <cstdlib>
#include <string.h>
#include <sstream>
#include <string> 
#include <thread>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

#define thread_parse 4096

//algorithm type instantiated
void *algo_type_0_lock(void *thread);

void *algo_type_1_lock(void *thread);

void *algo_type_2_lock(void *thread);

//atomic lock variables 
atomic<int> change_lock; 

atomic<int> id; 

atomic<bool> mutex_aquire; 

atomic<bool> if_flag_raise[thread_parse]; 

//User argument variables cast as int
int algo_type; 

int n; 

//variables keep track of threads
int thread_count;

long thread_iterator;

pthread_t thread_array[thread_parse];

int turn_id;

//inform the user of thier argument choice
void run_program(char* x, char* y){
    
    if(strcmp(x,"0") == 0){
        
        cout << "Option 0: Tournament-Tree Selected\n";
        
    }
    
    else if (strcmp(x,"1") == 0){
        
        cout << "Option 1: Test-And-Set Selected\n";
        
    }
    
    else if (strcmp(x,"2") == 0){
        
        cout << "Option 2: Fetch-And-Increment Selected\n";
        
    } 
    
    else{
        
        cout << "Input Error argv[1]: "
        "Please Input integer 0, 1, or 2 for algorithm selection";
        
    }
    
    cout << "Number N Threads: ";
    
    cout << y;
    
    cout << "\n\n";
    
}

//TT
void algo1(){
  
    thread_count = pthread_create(&thread_array[thread_iterator], NULL, 
    algo_type_0_lock, (void *)thread_iterator);

}

//TAS
void algo2(){
    
    thread_count = pthread_create(&thread_array[thread_iterator], NULL, 
    algo_type_1_lock, (void *)thread_iterator);

    
}

//FAI
void algo3(){
    
    thread_count = pthread_create(&thread_array[thread_iterator], NULL, 
    algo_type_2_lock, (void *)thread_iterator);

}

//accept arguments in main
int main(int argc, char *argv[]) {

    //check user inputs
    if (argc != 3)
	{
		cout << "ERROR: 3 arguments required to operate.\n"
        "1. Input the filename of this program\n"
        "2. Enter 0, 1, or 2 for TT, TAS and FAI respectively.\n"
        "3. Input the value for n\n"; 
        return -1;
	}

    //working with user inputs
	char* argument1 = argv[1];
    
    char* argument2 = argv[2];
    
    //user informed of choice    
    run_program(argument1, argument2);

    //cast argument 1
    int algo_type = atoi(argv[1]);

    //cast argument 2
    int n = atoi(argv[2]);

    //i iterator
    int i = 0;
    
    //atomic lock flag instantiated
    while(i < n){
        
        if_flag_raise[i] = false;
        
        ++i;
        
    }

    //local lock variables
    change_lock = 0;

    mutex_aquire = false;

    id = 0;

    int qe = algo_type;
    
    //creat threads for choice of algorithm
    for (thread_iterator = 0; thread_iterator < n; thread_iterator++) {

        cout << "Thread " << thread_iterator <<" is Created\n";

        int x = qe;
        
        //TT
        if(x == 0){
            
            algo1();
        
        }
       
        //TAS
        else if(x == 1){
            
            algo2();
        
        }
       
        //FAI
        else if(x == 2){
            
            algo3();
    
        }
       
       //error message checking user input
        else{
            
            cout << "\nError: Incorrect value for algorithm type detected."
            "Input must be 0, 1, or 2 for argv[1]\n";
            
        }
        
    }
    
    //thread join 
    for (thread_iterator = 0; thread_iterator < n; thread_iterator++) {
        
        pthread_join(thread_array[thread_iterator], NULL);
        
    }
    
    //exit
    pthread_exit(NULL);

    //end of program main
    return 0;
    
}

//TT
void *algo_type_0_lock(void *thread) {

    int thread_iterator = (long int)thread;

    if_flag_raise[thread_iterator] = true;

    change_lock = 1 - thread_iterator;

    while (if_flag_raise[1 - thread_iterator] && change_lock == 1 - thread_iterator) {
    
        sleep(0);

    }

    cout << "Thread " << thread_iterator << " has aquired the lock and has entered the critical section\n";

    if_flag_raise[thread_iterator] = false;

    pthread_exit(NULL);
    
}

//TAS
void *algo_type_1_lock(void *thread) {

    int thread_iterator = (long int)thread;

    while (atomic_exchange_explicit(&mutex_aquire, true, memory_order_acquire)) {
        
        sleep(0);

    }

    cout << "Thread " << thread_iterator << " has aquired the lock and has entered the Critical Section\n";

    atomic_store_explicit(&mutex_aquire, false, memory_order_release);

    pthread_exit(NULL);

}
    
//FAI
void *algo_type_2_lock(void *thread) {

    int thread_iterator = (long int)thread;

    turn_id = atomic_fetch_add_explicit(&id, 1, memory_order_acquire);

    while (change_lock != turn_id) {
       
        sleep(0);

    }

    cout << "Thread " << thread_iterator << " has aquired the lock and has entered the Critical Section\n";

    atomic_store_explicit(&change_lock, turn_id + 1, memory_order_release);

    pthread_exit(NULL);

}

//determine sign of Tournament-Tree
bool sign(int x){
    
    //value is even
    if (x%2 == 0)
    
        return true;
        
    //value is odd
    else if (x%2 == 1)
    
        return false;
        
    else
    
        return false;
        
}

//determine power of 2 vs n in Tournament-Tree
bool isPowerOfTwo(int n)
{
    if (n == 0)
    
        return false;
        
    while (n != 1){
        
        if (n%2 != 0)
        
            return false;
            
        n = n/2;
        
    }
    
    return true;
    
}

//Tournament-Tree Next Level
int next(int x, int level){

    int y = 0;
    
    //check if power of 2
    if (isPowerOfTwo(x) == true){
        
        //standard implementation
        return level%x;
        
    }
    
    else if (isPowerOfTwo(x) == false){
        
       //implementation for n is a non-power of 2 
       for(int i=0;i<x-1;++i){
           
           y = y + level%x;
           
       }
       
       return y + 1;
       
    }
    
    return level%x;
    
}