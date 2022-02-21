#see README for a manual compilation process
CC = g++
CFLAGS = -g
OBJ = open.o read.o stats.o

scheduler: $(OBJ) scheduler.cpp
	@echo "Creating the scheduler ..."
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp %.h
	@echo "Creating object files ..."
	$(CC) -c $< -o $@

clean:
	@echo "Deleting object and binary files ..."
	rm -rvf *.o scheduler
