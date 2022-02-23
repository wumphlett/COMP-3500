# Project 5
Separate compilation is demonstrated using the following multiple source code files.  
The following eight (8) source code files are derived from input.c  

1. open.h
2. open.cpp
3. read.h
4. read.cpp
5. stats.h
6. stats.cpp
7. scheduler.h
8. scheduler.cpp

How to manually compile the above multiple source code files?
```
g++ -c open.cpp
g++ -c read.cpp
g++ -c stats.cpp
g++ open.o read.o scheduler.cpp -o scheduler
```

How to run the executable file - scheduler?  
`./scheduler task.list`

How to compile using the Makefile?  
`make`

How to run?  
`./scheduler <file-name> [FCFS|RR|SRFT] [time_quantum]`
