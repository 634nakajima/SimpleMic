POBJS		= Module.o RoutingTable.o Coordinator.o MToken.o ModuleList.o Server.o Timer.o
CFLAGS = -std=c++11 -I ../liblo_new -I ../porttime

CC          	= g++

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean veryclean

all:	libmcore.a

clean:
	rm -f $(POBJS)   
	rm -f libmcore.a;

veryclean:
	rm -f $(POBJS)   
	rm -f libmcore.a;
	cd ../lib; rm -f libmcore.a;

libmcore.a:	$(POBJS)
	ar -rc libmcore.a $(POBJS)
	ranlib  libmcore.a

install:	libmcore.a
	cp libmcore.a ../lib
