*This project has been created as part of the 42 curriculum by zedurak.*

## Description

This project aims to simulate a specific number of philosophers and forks based on input arguments, observing whether they survive or perish according to their assigned eating and sleeping durations. Fundamentally, it is an educational project designed to teach the use of Threads and how to manage potential issues such as Deadlocks and Data Races during concurrent programming.

**Core Concepts:**

Thread: The smallest unit of execution within a Process that allows multiple functions to run simultaneously.

Process: An active instance of a program running on an operating system, possessing its own memory space.

Data Race: An inconsistency that occurs when multiple threads attempt to access and modify the same data at the same time.

Deadlock: A state where two or more threads are blocked indefinitely, each waiting for a resource (in this case, a fork) held by another.

**How it Works:**

In this project, each philosopher acts as a Thread running independently within a single Process, performing three main actions: eating, sleeping, and thinking. During these actions, threads must read or modify shared variables. For instance, while a philosopher is logging their status (e.g., "is eating") or updating their last meal time, another thread might attempt to access that same data simultaneously, leading to a Data Race.

Furthermore, because the philosophers are seated at a round table, an incorrect order of picking up forks can lead to a circular wait where every thread waits for another forever, resulting in a Deadlock. You can find the technical details and specific logic I applied to prevent these critical issues and ensure the program runs stably by examining the src file.

## Instructions

### Compilation

Navigate to the project directory and run:

```bash
cd philo
make
```

This will compile the project using GCC with thread and address sanitizers enabled for debugging.

### Execution

Run the program with the following arguments:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [times_each_must_eat]
```

**Arguments:**
- `number_of_philosophers`: Number of philosophers at the table (required)
- `time_to_die`: Time in milliseconds before a philosopher dies without eating (required)
- `time_to_eat`: Time in milliseconds it takes to eat (required)
- `time_to_sleep`: Time in milliseconds a philosopher sleeps (required)
- `times_each_must_eat`: Number of times each philosopher must eat before the simulation ends (optional; if omitted, simulation runs indefinitely until someone dies)

**Example:**

```bash
./philo 5 800 200 200 7
```

This simulates 5 philosophers with a death time of 800ms, eating time of 200ms, sleeping time of 200ms, and requires each to eat 7 times before completion.

### Cleaning

```bash
make clean      # Remove object files
make fclean     # Remove object files and executable
make re         # Rebuild from scratch
```

## Resources

The development process of this project was supported by the following technical documentations, videos, and peer learning:

- Video Tutorials:

[Unix Threads in C (Playlist)](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2): Comprehensive guide covering all necessary thread topics for this project.

[Introduction to Threads (pthreads)](https://www.youtube.com/watch?v=zOpzGHwJ3MU&t=2116s)

[Mutex in C](https://www.youtube.com/watch?v=ldJ8WGZVXZk)

- Articles & Documentation:

[The Dining Philosophers - Medium](https://medium.com/@jalal92/the-dining-philosophers-7157cc05315): Detailed analysis including all reference blog posts within.

[POSIX Threads Programming](https://web.archive.org/web/20210306083711/https://computing.llnl.gov/tutorials/pthreads/): Archive of the essential LLNL tutorial.

- Debugging Tools:

To resolve segmentation faults and memory leaks, I used -fsanitize=address -g flags. This enabled AddressSanitizer (ASan), a powerful memory error detector that precisely points out where leaks and invalid memory accesses occur in the code.

- Peer Learning: I also integrated insights and explanations shared by my peers throughout the learning process.

**AI Usage**

I use AI in every project to extract requirements and create a structured TO-DO list before I start coding according to those requirements. In this specific project, I utilized AI primarily for managing mutex locking and unlocking operations. I sought assistance when I couldn't identify the source of issues like infinite loops (deadlocks) or premature philosopher deaths. Through this process, I realized I was sometimes locking in the wrong places or forgetting to unlock entirely. Furthermore, AI helped me discover that I was missing death checks at certain critical points where a philosopher actually should have died. In general, I can say that I rely on AI as a primary resource whenever I encounter an error that I cannot resolve through traditional debugging.