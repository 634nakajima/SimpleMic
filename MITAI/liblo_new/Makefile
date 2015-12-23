#makefile for liblo
POBJS = address.o blob.o bundle.o message.o method.o pattern_match.o send.o server_thread.o server.o timetag.o

CFLAGS = -DHAVE_POLL

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean
all:	liblo.a

clean:
	rm -f $(POBJS) 
	rm -f liblo.a;

veryclean:
	rm -f $(POBJS) 
	rm -f liblo.a;
	cd ../lib; rm -f liblo.a;

liblo.a:	$(POBJS)
	ar -rc liblo.a $(POBJS)
	ranlib  liblo.a

install:	liblo.a
	cp liblo.a ../lib
