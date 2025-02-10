# CS F372 - Operating Systems Assignments

This repo has a couple of projects I worked on for my Operating Systems course. They dive into **multithreading, shared memory, and process scheduling**, all built in **C with pthreads**.

### What's Inside
1. **IPC Scheduler** - Plays around with interprocess communication and scheduling algorithms.
2. **Multi-Threaded Sudoku Solver** - Uses parallelism to solve Sudoku puzzles faster.

Both of these show how operating systems manage multitasking and efficiency.

---

## Project Breakdown

### IPC Scheduler (Process Scheduling & Multithreading)

This project does two main things:
- Uses **multithreading and shared memory** to quickly sum up a bunch of numbers.
- Simulates two process scheduling algorithms:
  - **Round Robin Scheduling** - Each process gets a time slice before switching.
  - **Pre-emptive Priority Scheduling** - Higher priority processes jump ahead of lower priority ones.

### Why It Matters
- **Multithreading makes things faster** by spreading work across multiple cores.
- **Shared memory lets processes talk to each other efficiently** without unnecessary copying.
- **Scheduling affects how systems run in the real world**, so understanding it is pretty useful.

---

### Multi-Threaded Sudoku Solver

This one is all about using **multi-threading to solve Sudoku puzzles faster**. Instead of checking each empty spot one by one, it:
- **Spawns multiple threads** to work in parallel.
- **Speeds up solving time**, especially for bigger puzzles.
- Got a **top 10 rank** in a class leaderboard for speed and accuracy.

---

## Running the Projects

Make sure you have **GCC and pthreads** installed. 
