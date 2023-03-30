#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;
// include additional necessary headers

// 2. Create necessary synchronization primitive(s)
mutex tcp_mutex;
condition_variable tcp_cv;
queue<int> line_num;
bool tcp_ready = false;

void query(int _count) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    // Lock the mutex and set shared variable to true
    unique_lock<mutex> ul(tcp_mutex);
    tcp_ready = true;

    // Notify response that it is ready
    tcp_cv.notify_one();

    // Unlock the mutex and print the statement
    cout << "[" << _count << "] SYN ... ";
    
}

void response() {
    // Should print "ACK"
    // Lock mutex and wait for shared variable to update
    unique_lock<mutex> ul(tcp_mutex);
    tcp_cv.wait(ul, [] { return tcp_ready; });

    // Update shared global variable
    tcp_ready = false;

    //Unlock mutex and print "ACK"
    cout << "ACK" << endl;
    
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */

    // 1. Get the count from cmd args
    int count = atoi(argv[1]);

    for (int i = 0; i < count; ++i) {
        // 3. Create two threads, one for "SYN" and the other for "ACK"
        thread t1(query, i);
        thread t2(response);

        // 5. Update the "query" and "response" functions to synchronize the output
        t1.join();
        t2.join();
    }
   
    return 0;
}