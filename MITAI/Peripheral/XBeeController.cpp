/*
 *  XBeeController.cpp
 *  
 *
 *  Created by kannolab on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "XBeeController.h"

XBeeController::XBeeController(Server *s) {
	rp = 0;
	wp = 0;
}

void XBeeController::addMethod(Module *m)
{
    m->addMethodToServer("/Audio", "b", stream, this);
}

void XBeeController::deleteMethod(Module *m)
{
    m->deleteMethodFromServer("/Audio", "b");
}

void XBeeController::parseData()
{
    printf("!\n");
	unsigned int value;
	unsigned char a64[8], a16[2], data[4];

	if (available() > 20) {
        while (available() > 0) {
            if (readData() == 0x7E) {
                for (int i = 0; i < 2; i++) {
                    readData();
                }
                data[0] = readData();
                a64[0] = readData();
                a64[1] = readData();
                a64[2] = readData();
                a64[3] = readData();
                a64[4] = readData();
                a64[5] = readData();
                a64[6] = readData();
                a64[7] = readData();
                
                a16[0] = readData();
                a16[1] = readData();
                
                data[1] = readData();
                
                value = readData();
                
                data[2] = readData();
                
				long sum = 0;
				for (int i=0; i<2; i++) {
					sum += data[i];
				}
				for (int i=0; i<8; i++) {
					sum += a64[i];
				}
				for (int i=0; i<2; i++) {
					sum += a16[i];
				}
				sum += value;

				data[3] = (0xFF - (sum & 0xFF));
				
				if(data[2] != data[3]) {
					printf("err:xbee\n");
					return;
				}
            }
        }
	}
}

char XBeeController::readData()
{
	char value;
	value = buf[rp];
	rp = rp+1%1024;
	return value;
}

int XBeeController::available()
{
	if (rp <= wp) {
		return (wp-rp);
	}else {
		return (1024+wp-rp);
	}
}

int XBeeController::stream(const char   *path, 
						   const char   *types, 
						   lo_arg       **argv, 
						   int          argc,
						   void         *data, 
						   void         *user_data)
{
	XBeeController *xbc = (XBeeController *)user_data;
	lo_blob b = (lo_blob)argv[0];
    char *dp = (char *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b);
	for (int i = 0; i < size; i++) {
		xbc->buf[xbc->wp] = *dp++;
		xbc->wp = xbc->wp+1%1024;
	}
	xbc->parseData();
    return 0;
}

XBeeController::~XBeeController()
{
}
