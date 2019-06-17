//-----------------------------------------------------------------------------
//
// MULTITHREADING DEMO 
//
// A quick tour of basic concurrency elements and use cases
// - Thread
// - Future
// - Atomic
// - Mutex/lock
// - Condition variable
//
// This demo use the v80 toolset and boost 1.59, if you want to
// try it out do not forget to set correctly the additional
// C++ and Linker include directories and thread, chrono and atomic
// input libraries in the project properties.
//
//-----------------------------------------------------------------------------

#define BOOST_THREAD_PROVIDES_FUTURE    // required to enable regular boost::future
#define _SECURE_SCL 0                   // required for boost on certain Windows compilers
#include <iostream>
#include <queue>
#include <string>
#include "boost/thread.hpp"
#include "boost/chrono.hpp"
#include "boost/atomic.hpp"
using namespace boost;
using namespace std;

#pragma region ThreadDemoUtils
int jobOfMs(int iDuration, int iIterations, int iIdentations)
{
  string wIdentation = "";
  for(int i = 0; i < iIdentations; ++i) { wIdentation += " "; }

  for(int i = 0; i < iIterations; ++i)
  {
    this_thread::sleep_for(chrono::milliseconds(iDuration));
    cout << wIdentation << i+1 << "..." << endl;
  }

  return iIterations * iDuration;
}

struct Worker
{
  int work(int iDuration, int iIterations, int iIdentations)
  {
    return jobOfMs(iDuration, iIterations, iIdentations);
  }
};


// ThreadDemoUtils demo

// int main()
// {
//   cout << "Return of first job : " << jobOfMs(200,2,0) << endl;
//   cout << "Return of first job : " << jobOfMs(200,3,2) << endl;
//   Worker worker1;
//   Worker worker2;
//   cout << "Return of worker1 job : " << worker1.work(200,4,4) << endl;
//   cout << "Return of worker2 job : " << worker2.work(200,5,6) << endl;
//   getchar();
// }


#pragma endregion ThreadDemoUtils


//---------------------------------------------------
// Threads
//---------------------------------------------------

// int main()
// {
//   cout << "Current thread id : " << this_thread::get_id() << endl;
//   cout << "Theoretical max number of thread : " << thread::hardware_concurrency() << endl;
//   
//   Worker worker;
//   
//   // Start a thread with a free function and arguments
//   boost::thread thread1(jobOfMs, 1000, 3, 0);
// 
//   // Bind a thread to a method of a specific object instance
//   thread thread2(bind(&Worker::work, &worker, 500,5,8));
//   
// //   cout << "thread1.join() blocks the main thread" << endl;
// //   thread1.join();
// //   cout << "thread1 work finished" << endl;
// // 
// //   cout << "thread2.join() blocks the main thread" << endl;
// //   thread2.join();
// //   cout << "thread2 work finished" << endl;
// 
//   // join() vs detach()
//   thread1.detach();  
//   cout << "thread1 working" << endl;
//   cout << "thread2.join() blocks the main thread" << endl;
//   //thread2.join();
//   cout << "thread2 work finished" << endl;
// 
//   cout << "Program ends" << endl;
//   //getchar();
// }



//---------------------------------------------------
// Futures
//---------------------------------------------------

// #define BOOST_THREAD_PROVIDES_FUTURE 
// Don't forget to define this BEFORE #include "boost/thread.hpp"
// 
// int main()
// {
//   Worker worker;
// 
//   // Bind future to a method of a specific object instance
//   future<int> future1 = async(bind(&Worker::work, &worker, 1500,2,4));
//   
//   // Bind future to a free function
//   future<int> future2 = async(launch::deferred, bind(jobOfMs, 500,8,8));
//   
//   cout << "future2 returns : " << future2.get() << endl;
//   cout << "future1 returns : " << future1.get() << endl;
// 
//   getchar();
// }



//---------------------------------------------------
// Waiting
//---------------------------------------------------
/*
int main()
{
  while("the CPU burns...")
  {
    this_thread::sleep_for(chrono::milliseconds(1));  // ~0%
    //this_thread::sleep_for(chrono::nanoseconds(1));   // ~3%
    //this_thread::yield();                             // ~8%
  }
}
*/


//---------------------------------------------------
// Atomic
//---------------------------------------------------

atomic<std::string> atomicValue;
std::string normalValue;

 void work()
 {
   cout << "Thread " << this_thread::get_id() << " started!" << endl;
   short flip;
   this_thread::sleep_for(chrono::seconds(1));
   
   while("playing with the odds...")
   {
     if(++flip % 2) 
     { 
       normalValue = "ODD"; 
       atomicValue.store("ODD");
     }
     else 
     { 
       normalValue = "EVEN"; 
       atomicValue.store("EVEN");
     }
   }
 }
 
 int main()
 {
   for(int i = 0; i < 20; ++i) { thread(work).detach(); }

   while("you were away...")
   {
     cout << atomicValue.load() << "\t" << normalValue << endl;
     this_thread::sleep_for(chrono::seconds(1));
   }
}



//---------------------------------------------------
// Lock & mutex
//---------------------------------------------------
/*
struct SyncWorker
{
  // Really important to declare default and copy constructors
  // to guarantee that member mutexes are not copied
  SyncWorker(const SyncWorker& other){}
  SyncWorker(){}
  
  mutex mMutex;
  vector<int> mVector;

  void work()
  {
    cout << "Thread " << this_thread::get_id() << " started!" << endl;
    this_thread::sleep_for(chrono::seconds(1));

    while("Until the program crashes...")
    {
      // "Classic" lock-unlock
      //mMutex.lock();
      //mVector.push_back(mVector.size()); cout << '.';
      //mMutex.unlock();

      // Try lock method
      //if(mMutex.try_lock)
      //{
      //  mVector.push_back(mVector.size()); cout << '.';
      //  mMutex.unlock();
      //}

      // RAII lock
      //lock_guard<mutex> lock(mMutex);
      //mVector.push_back(mVector.size()); cout << '.';
    }
  }

};

int main()
{
  SyncWorker worker;
  for(int i = 0; i < 20; ++i) { thread(bind(&SyncWorker::work, &worker)).detach(); }
  getchar();
}
*/


//---------------------------------------------------
// Timed mutex
//---------------------------------------------------
/*
timed_mutex timedMutex;
vector<int> sharedVector;

void work()
{
  while("we still have time...")
  {
    cout << this_thread::get_id() << "\twakes up and try to access the resource..." << endl;
    if(timedMutex.try_lock_for(chrono::seconds(2)))
    {
      // Was able to get the lock
      cout << this_thread::get_id() << "\tLOCKS THE RESOURCE!" << endl;
      this_thread::sleep_for(chrono::seconds(5));
      sharedVector.push_back(sharedVector.size());
      cout << this_thread::get_id() << "\treleases the resource and goes to sleep for 2 seconds" << endl;
      timedMutex.unlock();
      this_thread::sleep_for(chrono::seconds(2));
    }
    else
    {
      // Attempt timed out
      // Sleep or do something else
      cout << this_thread::get_id() << "\ttimed out, will sleep for 10 seconds..." << endl;
      this_thread::sleep_for(chrono::seconds(10));
    }
  }
}

int main()
{
  for(int i = 0; i < 3; ++i) { thread(work).detach(); }
  getchar();
}
*/


//---------------------------------------------------
// Recursive mutex
//---------------------------------------------------
/*
recursive_mutex recursiveMutex;
vector<int> sharedVector;

void workOnSharedResource()
{
  // Thread might lock a second time here
  lock_guard<recursive_mutex> lock(recursiveMutex);
  sharedVector.push_back(sharedVector.size());
  cout << '.';
}

void preliminaryWork()
{
  // Something that does not require a lock
  // ...
  this_thread::sleep_for(chrono::milliseconds(1));

  workOnSharedResource();
}

void work()
{
  cout << "Thread " << this_thread::get_id() << " started!" << endl;
  this_thread::sleep_for(chrono::seconds(1));

  bool flip = true; 
  while("the program has not crashed yet...")
  {
    // Thread locks once here
    lock_guard<recursive_mutex> lock(recursiveMutex);
    if(flip) { workOnSharedResource(); }
    else     { preliminaryWork(); }
    flip = !flip;
  }
}

int main()
{
  for(int i = 0; i < 20; ++i) { thread(work).detach(); }
  getchar();
}

// NOTE:
// If manually locking/unlocking the recursive mutex, it must
// be unlock as many times as it was locked
*/


//---------------------------------------------------
// Shared mutex
//---------------------------------------------------
/*
shared_mutex sharedMutex;
vector<int> sharedVector;

void analyze()
{
  while("the earth sleeps...")
  {
    int localValue = -1;
  
    {
      shared_lock<shared_mutex> lock(sharedMutex);
      if(sharedVector.size()) 
      {
        localValue = sharedVector.back();
      }
    }

    if(localValue < 0) { return; }

    bool isPrime = true;
    for(int i = 2; i < localValue; ++i)
    {
      if(!(localValue % i)) { isPrime = false; }
    }

    if(isPrime) { cout << endl << localValue << " is prime" << endl; }

  }
}

void produce()
{  
  while("we still have chocolate chips...")
  {
    unique_lock<shared_mutex> lock(sharedMutex);
    sharedVector.push_back(sharedVector.size());
    cout << '+';
  }
}

void consume()
{
  while("we still have cookies...")
  {
    unique_lock<shared_mutex> lock(sharedMutex);
    if(sharedVector.size() > 10 && sharedVector.size() % 2) 
    { 
      sharedVector.erase(sharedVector.begin() + (sharedVector.size() / 2));
      cout << '-';
    }
  }
}

int main()
{
  for(int i = 0; i < 4; ++i) { thread(produce).detach(); }
  for(int i = 0; i < 2; ++i) { thread(analyze).detach(); }
  for(int i = 0; i < 2; ++i) { thread(consume).detach(); }

  getchar();
}
*/


//---------------------------------------------------
// Condition variable
//---------------------------------------------------
/*
condition_variable sharedConditionVariable;
shared_mutex sharedQueueMutex;
std::queue<std::string> sharedQueue;

void pushQueue(std::string& iInput)
{
  unique_lock<shared_mutex> lock(sharedQueueMutex); 
  sharedQueue.push(iInput); 
}

void popQueue(std::string& oOutput)
{
  unique_lock<shared_mutex> lock(sharedQueueMutex); 
  oOutput = sharedQueue.front();
  sharedQueue.pop();
}

bool noWork()
{
  shared_lock<shared_mutex> lock(sharedQueueMutex); 
  return sharedQueue.empty();
}

struct SyncWorker
{
  void work()
  {
    // Worker initialization
    std::string task = "";
    mutex localConditionMutex;
    unique_lock<mutex> lock(localConditionMutex);
    cout << "Worker " << this_thread::get_id() << " ready" << endl;

    // Main worker loop
    while(task != "exitworker")
    {
      // Standing by
      while(noWork()) { sharedConditionVariable.wait(lock); }

      // Work occurs here
      popQueue(task);
      cout << "Worker "<< this_thread::get_id() << " crunching : " << task << endl;
      this_thread::sleep_for(chrono::seconds(3));
    }

    cout << "Worker "<< this_thread::get_id() << " stopped working " << endl;
  }
};

int main()
{
  // Init program
  SyncWorker worker1, worker2, worker3;

  thread thread1(bind(&SyncWorker::work, &worker1));
  thread thread2(bind(&SyncWorker::work, &worker2));
  thread thread3(bind(&SyncWorker::work, &worker3));

  pushQueue(std::string("Pre-loaded work 1"));
  pushQueue(std::string("Pre-loaded work 2"));
  pushQueue(std::string("Pre-loaded work 3"));
  pushQueue(std::string("Pre-loaded work 4"));
  pushQueue(std::string("Pre-loaded work 5"));

  // Trigger CV to start processing initial work
  sharedConditionVariable.notify_all();

  // Main program loop
  std::string textInput;
  while(textInput != "exitprogram")
  {
    cin >> textInput;
    pushQueue(textInput);
    sharedConditionVariable.notify_one();
  }  

  // Clean up
  pushQueue(std::string("exitworker"));
  pushQueue(std::string("exitworker"));
  pushQueue(std::string("exitworker"));
  sharedConditionVariable.notify_all();

  if(thread1.joinable()) { thread1.join(); }
  if(thread2.joinable()) { thread2.join(); }
  if(thread3.joinable()) { thread3.join(); }

  while(!sharedQueue.empty()) { sharedQueue.pop(); }
}
*/