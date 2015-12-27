MITAIAPP	= main.o

PROG		= SimpleMic
INCLUDES 	= -I./MITAI/Core -I ./MITAI/Services -I ./MITAI/Peripheral -I./MITAI/liblo_new -I./MITAI/porttime
LIBS 		= -L./MITAI/lib -lmcore -lmservice -lmper -llo -lportaudio -lpt -lasound -lpthread
CC          	= g++ -std=c++11

.PHONY: mitai clean veryclean

all:	mitai myApp

mitai:
	cd ./MITAI/; make;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

myApp: $(MITAIAPP)
	$(CC) -o $(PROG) $(MITAIAPP) $(LIBS) $(INCLUDES)

clean:
	rm -f *.o   
	cd ./MITAI/; make clean;

veryclean:
	make clean
	rm -f $(PROG)
	cd ./MITAI/; make veryclean;
