POBJS		= ADC.o DAC.o
CFLAGS = -std=c++11 -I ../Core -I../Peripheral -I ../liblo_new -I ../porttime

CC          	= g++

.cpp.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean veryclean

all:	libmservice.a

clean:
	rm -f $(POBJS)   
	rm -f libmservice.a;

veryclean:
	rm -f $(POBJS)   
	rm -f libmservice.a;
	cd ../lib; rm -f libmservice.a;

libmservice.a:	$(POBJS)
	ar -rc libmservice.a $(POBJS)
	ranlib  libmservice.a

install:	libmservice.a
	cp libmservice.a ../lib
