MITAIAPP	= main.o

PROG		= SimpleMic
INCLUDES 	= -I./MITAI/Core -I ./MITAI/Services -I./MITAI/liblo_new -I./MITAI/porttime
LIBS 		= -L./MITAI/lib -lmcore -lmservice -llo -lportaudio -lpt -lasound -lpthread
CC          	= g++

.PHONY: mitai clean

all:	mitai myApp

mitai:
	cd ./MITAI/; make;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

myApp: $(MITAIAPP)
	$(CC) -o $(PROG) $(MITAIAPP) $(LIBS) $(INCLUDES)

clean:
	rm -f *.o   
	rm -f $(PROG)