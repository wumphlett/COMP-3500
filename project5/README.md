# Project 5
Separate compilation is demonstrated using the following multiple source code files.  
The following eight (8) source code files are derived from input.c  

1. open.h
2. open.c
3. read.h
4. read.c
5. stats.h
6. stats.c
7. scheduler.h
8. scheduler.c

How to manually compile the above multiple source code files?
```
$gcc -c open.c
$gcc -c read.c
$gcc -c print.c
$gcc open.o read.o print.o scheduler.c -o scheduler
```

How to run the executable file - scheduler?  
`./scheduler task.list`

How to compile using the Makefile?  
`make`

How to run?  
`./scheduler <file-name> [FCFS|RR|SRFT] [time_quantum]`
