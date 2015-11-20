# CS_466
Project files for the CS 446 Operating Systems course (Fall 2015). (I accidently named this repository wrong....)  

These projects are have been designed with the purpose of simulating a CPU's process scheduler. A program generator is used to generate a series of tasks (I/O operations, run operations, new applications, etc.) and a config file is used to set various cycle times, output paths, etc.  

Sim_01 is designed to accept one application and execute each operation linearly within its own thread. For the most part, this project was pretty terribly designed. Although it turned out this program will accept multiple applications and execute them with a FIFO algorithm.  

Sim_02 went through a lot of redesigning and more closely followed OOP principles as the scope of the simulation increased. This simulation features batch processing and implements FIFO (First in, first out), SJF (Shortest job first), and SRTF-N (Shortest remaining time first - non-pre-emptive) CPU scheduling algorithms.  

Sim_03 features true multi-threaded programming. As in, it will no longer wait for I/O operation threads to finish before continuing onto the next task. Instead, when an application executes an I/O operation, that application will become "blocked" and a new application will execute. Once an application becomes unblocked, it will return to the processing queue. This simulation features RR (Round robin), FIFO-P (First in first out - with pre-emption), and SRTF-P (Shortest remaining time first - with pre-emption). It will also make use of a quantum time, so operations are no longer executed from start to finish.

PA1_ug features process creation and system calls by simulating a family tree. Not particularly interesting. Although it is programmed in C!