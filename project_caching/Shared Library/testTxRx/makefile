#Make file

HDIR =/usr/local/include/caching
EFLAGS = -I$(HDIR)

CC = g++
CFLAGS = -O3 -Wall -std=c++11 -g
LIB = -lcaching -lgsl -lgslcblas -lm


TestTxRx :
	$(CC) $(CFLAGS) -o $@ TestTxRx.cpp $(LIB) $(EFLAGS)
	clear
	./TestTxRx

clean :
	rm -r TestTxRx
