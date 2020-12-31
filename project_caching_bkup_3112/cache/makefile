#Make file

HDIR =/usr/local/include/caching
#CACHEDIR =/CachingFile/cache/UserCache
CACHEDIR =UserCache
EFLAGS = -I$(HDIR)

CC = g++
CFLAGS = -O3 -Wall -std=c++11 -g
LIB = -lcaching -lgsl -lgslcblas -lm


MakeCache :
	mkdir $(CACHEDIR)
	$(CC) $(CFLAGS) -o $@ make_cache.cpp $(LIB) $(EFLAGS)
	clear
	./MakeCache

clean :
	rm -r $(CACHEDIR)
	rm -r MakeCache
