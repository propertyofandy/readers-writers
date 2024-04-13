#ifndef SHARED_RESOURCE_HPP_
#define SHARED_RESOURCE_HPP_

#include <vector>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <cstdlib>
#include <mutex>
#include <memory>
#include <thread>
#include <iostream> 
#include <chrono>


#define ENDL std::cout << std::endl

class ISharedResource
{
public:
    virtual std::vector<int> critical_read() = 0;
    virtual void critical_write(int item) = 0;
};

template <class T>
class SharedResource
{
private:
    ISharedResource *sharedData;
    //void (ISharedResource::*critical_write)(int);
    //std::vector<int> (ISharedResource::*critical_read)();
    int number_of_readers;
    int number_of_writers;
    int max_reads;
    int max_writes;

    std::mutex resources_mutex;
    std::mutex reader_mutex;
    std::condition_variable cond;
    int reader_count; 


void reader(int thread_id){
    static int n = 0;
    // unique lock is a wrapper that handles exceptions from mutex
    // and also uses RAII so it will unlock when its popped off stack
    { 
        //scope of mutex lock
        std::unique_lock<std::mutex> lock(reader_mutex);
        reader_count++;
        if(reader_count == 1)
            resources_mutex.lock();
    }

    std::cout   << "thread: "
                << thread_id
                << " is reading, times read: " 
                << n++ 
                << std::endl; 
    {
        std::unique_lock<std::mutex> lock(reader_mutex);
        for(auto item : ((T*)sharedData)->critical_read()){
            std::cout << item << ", ";
        }
        ENDL; 
    }
    
    
    {
        std::unique_lock<std::mutex> lock(reader_mutex);
        std::cout   << "thread: "
                    << thread_id
                    << " finished reading"
                    << std::endl; 
        
        reader_count--;
        if(reader_count == 0) resources_mutex.unlock();
    }

}


void write(int thread_id){
    resources_mutex.lock();

    int number_to_add = rand()%100;

    std::cout   << "thread: "
                << thread_id
                << " writing "
                << number_to_add
                << std::endl;

    ((T*)sharedData)->critical_write(number_to_add);

    std::cout   << "thread: "
                << thread_id
                << " finished writing"
                << std::endl;

    resources_mutex.unlock(); 
}


void readFunction(int thread_id){
    static int i = 0;

    for (; i < max_reads; i++)
    {
        reader(thread_id); 
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    
}

void writeFunction(int thread_id){
    static int i = 0;

    for (; i < max_writes; i++)
    {
        write(thread_id); 
        std::this_thread::sleep_for(std::chrono::microseconds(50));

    }
}

public:
SharedResource(ISharedResource *s, int num_readers, int num_writers, int max_r, int max_w){
            sharedData = s;
            number_of_readers = num_readers;
            number_of_writers = num_writers; 
            max_reads = max_r;
            max_writes = max_w;
}

void start(){
        // declare vector of threads
    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    // initialize threads
    for (size_t i = 0; i < number_of_readers; i++)
    {
        readers.push_back(std::thread(&SharedResource::readFunction, this, i));
    }

    for (size_t i = 0; i < number_of_writers; i++)
    {
        writers.push_back(std::thread(&SharedResource::writeFunction, this, i));
    }

    for(auto& t : readers) t.join();
    for(auto& t : writers) t.join();
}

};




#endif