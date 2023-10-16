#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

const int N = 1000000;

int Race()
{
    int shared_val = 0;

    auto critical_section = [&shared_val]() {
        for (int i = 0; i < N; ++i) {
            shared_val++; // this is three operations, read value, increment and write back
        }
    };

    // threads executing the critical code section without any synchronization
    std::thread t1(critical_section);
    std::thread t2(critical_section);

    t1.join();
    t2.join();

    return shared_val;
}

int Atomic()
{
    // atomic integer
    std::atomic<int> shared_val(0);

    // Function to increment atomic int
    auto critical_section = [&shared_val]() {
        for (int i = 0; i < N; ++i) {
            shared_val++; // std::atomic overloads operator++ and atomically increments the value, (sum = sum + 1) is not atomic
        }
    };
    // threads executing the critical code section
    std::thread t1(critical_section);
    std::thread t2(critical_section);

    t1.join();
    t2.join();

    return shared_val;
}

int Mutex()
{
    int shared_val = 0;

    // Create a mutex
    std::mutex m;

    // Function to increment atomic int
    auto critical_section = [&shared_val, &m]() {
        for (int i = 0; i < N; ++i) {
            std::lock_guard<std::mutex> g(m);
            shared_val++; // std::atomic overloads operator++ and atomically increments the value, (sum = sum + 1) is not atomic
        }
    };
    // threads executing the critical code section
    std::thread t1(critical_section);
    std::thread t2(critical_section);

    t1.join();
    t2.join();

    return shared_val;
}


int main()
{
    cout << "race: " << Race() << endl;
    cout << "atomic: " << Atomic() << endl;
    cout << "mutex: " << Mutex() << endl;
    return 0;
}
