POBJS		= StreamingBuffer.o Volume.o
CFLAGS = -std=c++11 -I ../Peripheral

CC          	= g++

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean veryclean

all:	libmper.a

clean:
	rm -f $(POBJS)   
	rm -f libmper.a;

veryclean:
	rm -f $(POBJS)   
	rm -f libmper.a;
	cd ../lib; rm -f libmper.a;

libmper.a:	$(POBJS)
	ar -rc libmper.a $(POBJS)
	ranlib  libmper.a

install:	libmper.a
	cp libmper.a ../lib
