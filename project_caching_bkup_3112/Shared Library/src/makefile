#Makefile for SourceBlock

ODIR =obj

HDIR =../include/caching
LDIR =../lib
#FILEDIR = ../CachingFile/

STD_LDIR =/usr/local/lib
STD_HDIR =/usr/local/include/caching
STD_PCDIR =/usr/local/lib/pkgconfig
#STD_FILEDIR = /CachingFile/
#STD_DIR = /


CC = g++
CFLAGS = -O3 -Wall -std=c++11 -g -lgsl -lgslcblas -lm -fPIC -c
EFLAGS = -I$(HDIR)

_DEPS_H = CheckFunction.h CodingDecodingData.h ConflictGraph.h Conversions.h DataDefinition.h DecodingInput.h EnvironmentSetup.h PC.h FuncsFromMain.h grasp.h heapSort.h psdes.h randomHandler.h randomUniform.h TxRx.h utilityForTesting.h gr_complex.h
DEPS_H = $(patsubst %,$(HDIR)/%,$(_DEPS_H))
#hgcc.h

DEPS_H_STD = $(patsubst %,$(STD_HDIR)/%,$(_DEPS_H))

PCFILE = ../pkgconfig/caching.pc


_OBJ = psdes.o CheckFunction.o heapSort.o randomUniform.o randomHandler.o DecodingInput.o EnvironmentSetup.o FuncsFromMain.o ConflictGraph.o grasp.o  CodingDecodingData.o utilityForTesting.o Conversions.o TxRx.o PC.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#hgcc.o







SourceBlock : $(ODIR) $(OBJ)
	$(CC) -shared -o $(LDIR)/libcaching.so $(OBJ) #-shared Produce a shared object which can then be linked with other objects to form an executable.
	


install :
	sudo cp $(LDIR)/libcaching.so $(STD_LDIR)
	sudo chmod 0755 $(STD_LDIR)/libcaching.so # permesso 0755: imposta lettura, scrittura ed esecuzione per il proprietario, e lettura ed esecuzione per gruppo ed altri (comune per le directory e per i file eseguibili)
	sudo mkdir $(STD_HDIR)
	sudo cp $(DEPS_H) $(STD_HDIR)
	sudo chmod 0755 $(DEPS_H_STD)
	#sudo mkdir $(STD_PCDIR)
	sudo cp $(PCFILE) $(STD_PCDIR)
	sudo chmod 0755 $(STD_PCDIR)/caching.pc
	sudo ldconfig
	#sudo cp -r $(FILEDIR) $(STD_DIR)
	#sudo chmod -R 0777 $(STD_FILEDIR)



clean :
	rm -r $(ODIR)
	rm $(LDIR)/libcaching.so
	sudo rm $(STD_LDIR)/libcaching.so
	sudo rm -r $(STD_HDIR)
	sudo rm $(STD_PCDIR)/caching.pc
	#sudo rm -r $(STD_FILEDIR)










$(ODIR) :
	mkdir $(ODIR)

$(ODIR)/PC.o : $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ PC.cpp $(EFLAGS)

$(ODIR)/psdes.o : $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ psdes.cpp $(EFLAGS)

$(ODIR)/CheckFunction.o : $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ CheckFunction.cpp $(EFLAGS)

$(ODIR)/heapSort.o : $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ heapSort.cpp $(EFLAGS)

$(ODIR)/randomUniform.o : $(ODIR)/psdes.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ randomUniform.cpp $(EFLAGS)

$(ODIR)/randomHandler.o : $(ODIR)/randomUniform.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ randomHandler.cpp $(EFLAGS)

$(ODIR)/DecodingInput.o : $(ODIR)/CheckFunction.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ DecodingInput.cpp $(EFLAGS)

$(ODIR)/EnvironmentSetup.o : $(ODIR)/CheckFunction.o $(ODIR)/randomHandler.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ EnvironmentSetup.cpp $(EFLAGS)

$(ODIR)/FuncsFromMain.o : $(ODIR)/utilityForTesting.o $(ODIR)/randomHandler.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ FuncsFromMain.cpp $(EFLAGS)

$(ODIR)/ConflictGraph.o : $(ODIR)/CheckFunction.o $(ODIR)/randomHandler.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ ConflictGraph.cpp $(EFLAGS)

$(ODIR)/grasp.o : $(ODIR)/randomHandler.o $(ODIR)/heapSort.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ grasp.cpp $(EFLAGS)

#$(ODIR)/hgcc.o : $(DEPS_H)
#	$(CC) $(CFLAGS) -o $@ hgcc.cpp $(EFLAGS)

$(ODIR)/utilityForTesting.o : $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ utilityForTesting.cpp $(EFLAGS)

$(ODIR)/CodingDecodingData.o : $(ODIR)/utilityForTesting.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ CodingDecodingData.cpp $(EFLAGS)

$(ODIR)/Conversions.o : $(ODIR)/utilityForTesting.o $(ODIR)/randomHandler.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ Conversions.cpp $(EFLAGS)

$(ODIR)/FuncsFromMain.o : $(ODIR)/utilityForTesting.o $(ODIR)/randomHandler.o $(ODIR)/Conversions.o $(ODIR)/DecodingInput.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ FuncsFromMain.cpp $(EFLAGS)

$(ODIR)/TxRx.o : $(ODIR)/utilityForTesting.o $(ODIR)/randomHandler.o $(ODIR)/Conversions.o $(DEPS_H)
	$(CC) $(CFLAGS) -o $@ TxRx.cpp $(EFLAGS)
