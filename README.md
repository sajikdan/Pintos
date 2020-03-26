# Pintos
Operating System.
Pintos is a simple OS which can boot, execute an application, and power off.

# Project 1 User Programs
Objective: make Pintos execute user programs properly.

To do so, the following have been implemented:
- System calls
- System call handlers
- Argument passing
- User stack

Understanding of the following concepts were required:
- Code level flow of Pintos.
- Virtual memory in Pintos.
- Processes and threads in Pintos.
- Pages in Pintos.
- System calls.
- Process termination messages in Pintos.
- Argument passing.
- Accessing user memory.

# Project 2 User Programs
Objective: implement system calls about file system.

To do so, the following have been implemented:
- File descriptor.
- File system System calls.
- Memory management.
- Denying writes to executable files.
- Protect critical section / synchronization.

Understanding of the following concepts were required:
- Base file system of Pintos and its limitations.
- File access at Kernel.
- File system calls.
- Process execution and load to memory.
- Executable file of running program.
- Critical sections and synchronization (locks and semaphores).

# Project 3 Threads
Objective:
- Reduce CPU usage by substituting the Busy Waiting method of sleeping threads to Alarm Clock method.
- Process scheduling based on priority, while avoiding starvation using aging.

To do so, the following have been implemented:
- Alarm Clock.
- Priority Scheduling using aging.
- Advanced Scheduler.

Understanding of the following concepts were required:
- Processes and threads in Pintos.
- Thread statuses and thread switching.
- Synchronization using locks and semaphores in critical sections.
- Alarm clock, timers, interrupts and thread yielding.
- Priority scheduling and aging
- Default scheduler of Pintos (round robin).
- Advanced scheduler using Multi-Level Feedback Queue (MLFQ).
- Calculating priority, niceness, recent CPU usage and load average using fixed point arithmetic.
