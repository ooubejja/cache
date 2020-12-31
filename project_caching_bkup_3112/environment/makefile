#Make file

HDIR =/usr/local/include/caching
EFLAGS = -I$(HDIR)

CC = g++
CFLAGS = -O3 -Wall -std=c++11 -g
LIB = -lcaching -lgsl -lgslcblas -lm


Environment :
	$(CC) $(CFLAGS) -o $@ environment.cpp $(LIB) $(EFLAGS)
	clear
	./Environment

clean :
	rm -r Environment
	rm -r environment_file