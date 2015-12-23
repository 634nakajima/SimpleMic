#makefile for porttime
POBJS = ptlinux.o



CC=gcc

.PHONY:	clean veryclean

all:	libpt.a

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm -f $(POBJS) 
	rm -f libpt.a;

veryclean:
	rm -f $(POBJS) 
	rm -f libpt.a;
	cd ../lib; rm -f libpt.a;

libpt.a:	$(POBJS)
	ar -rc libpt.a $(POBJS)
	ranlib  libpt.a

install:	libpt.a
	cp libpt.a ../lib
