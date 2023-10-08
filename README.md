# Locks

⚠️ WARNING

Work in progress, keep you expectations low!



## Ways to acquire locks on shared resources

## Reasons for implementing a lock mechanism

- Efficiency

- Correctness

- Both

## Terminology

- Optimistic locking

  Allows multiple users to update, only one will success the others are informed about a the conflict (race condition)

- Pessimistic locking

- Centralized locking:
  - Consensus-based locking:
    ex: ZooKeeper (Zab), etcd (Raft), Redis?

- Monotonic time

- Critical Section

  A section of the code that are accessing a shared resource

- Starvation

  A thread is unable to make progress for a long period of time

- Race condition

  Multiple threads are changing some data at the same time and the outcome is nondeterministic

    example:

    ```cpp
    int shared_val = 0;

    auto critical_section[&shared_val]() {
        shared_val++; // this is three operations, read value, increment and write back
    }

    // threads executing the critical code section without any synchronization
    std::thread t1(critical_section);
    std::thread t2(critical_section);
    ```

## Regular single machine locks

### Atomic operations

An operation on an object that is guaranteed to be executed in its completeness without any other thread having access to to that object until the operation has been completed.

The CPU has extra instructions for achieving exclusive access to a memory space in the cache. Atomic operations costs a few extra CPU cycles compared to non-atomic `normal` operations.

example:

```cpp
// atomic integer
std::atomic<int> shared_val(0);

// Function to increment atomic int
auto critical_section[&shared_val]() {
    shared_val++; // std::atomic overloads operator++ and atomically increments the value, (sum = sum + 1) is not atomic
}
// threads executing the critical code section
std::thread t1(critical_section(shared_val));
std::thread t2(critical_section(shared_val));
```

### Semaphore

A shared unsigned int value used as a signaling mechanism between multiple threads, only one thread at the time can change the semaphore value:
- implemented as a shared unsigned int value
- machine-independent

### Mutual exclusion (mutex)

A special form of Semaphore providing mutual exclusion to a critical section of code:
- Implemented as a class
- machine-independent
- waiting threads can lead to wastage of the CPU cycles
- costly in terms of CPU cycles compared to atomic operations

```cpp
long long int shared_val{0};

// Create a mutex
std::mutex m;

// Function to increment atomic int
void critical_section(std::mutex &m, long long int &val) {
    std::lock_guard<std::mutex> g(m);
    val++;
}
// threads executing the critical code section
std::thread t1(critical_section(m, shared_val));
std::thread t2(critical_section(m, shared_val));
```

## Distributed locks