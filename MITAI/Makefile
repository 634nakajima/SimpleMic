INCLUDES 	= -I./Core -I./liblo_new -I./porttime
CC          	= g++

.PHONY: liblo porttime mcore mservice mper clean veryclean

all:	liblo porttime mcore mservice mper

liblo:
	cd liblo_new; make; make install;

porttime:
	cd porttime; make; make install;

mcore:
	cd Core; make; make install;

mservice:
	cd Services; make; make install;
mper:
	cd Peripheral; make; make install;

clean:
	cd liblo_new; make clean;
	cd porttime; make clean;
	cd Core; make clean;
	cd Services; make clean;
	cd Peripheral; make clean;

veryclean:
	cd liblo_new; make veryclean;
	cd porttime; make veryclean;
	cd Core; make veryclean;
	cd Services; make veryclean;
	cd Peripheral; make veryclean; 
	