# POSIX-threads
This program simulates POSIX threads and their advanced synchronization 
features.

This is a multi-threaded simulation of a post office environment to manage 
customer service by clerks that employes synchronization mechanisms such as 
mutexes and condition variables to handle concurrent access and avoid race 
conditions. Additionally, the simulation shows patrons arriving at the post office, and threads simulate service interactions where patrons may have to wait for an available clerk.

The following are some of the key features learned in this project:
* Multithreading: Utilized POSIX threads (pthreads) to simulate multiple 
patrons arriving at the post office and being served by a fixed number of 
clerks.
* Synchronization: Implemented mutexes and condition variables to manage 
access to shared resources, such as the number of available clerks and 
customer queue.
* File I/O: Read input data from a file containing patron arrival delays 
and service times to simulate real-world scenarios.
* Dynamic Memory Allocation: Managed dynamic memory for storing patron 
data and messages using 'malloc' and 'free'.
* Efficient Resource Management: Patrons had to wait if no clerks were 
available, using condition variables to efficiently handle wait times and 
service allocation.

Some of the key contributions made in this programs are:
* Developed the main logic for initializing threads and managing customer 
service.
* Ensured synchronization between customer threads and clerk availability, 
preventing race conditions and deadlocks.
* Designed and implemented a report summarizing patron activity, including 
the number of patrons served, waiting times, and resource utilization.
* Enhanced error handling and robustness, ensuring the program gracefully 
handles input errors, memory allocation failures, and synchronization 
issues.

To run this code (*1):
1. gcc main.c -o main -lpthread
2. ./main X
\n where X is the number of threads. In this programs, it represents the number of clerks; it could be 1, 2, 3, ...

For any question, please send me a LinkedIn message at https://www.linkedin.com/in/kevin-mejia-bonilla/!


(*1) The users needs to have the C environment installed before running this code and may need to install some pthread packages.