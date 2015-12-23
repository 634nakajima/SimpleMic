#ifndef _Serial_h
#define _Serial_h

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <vector>
#include <dirent.h>

#define BAUDRATE B9600           
#define MODEMDEVICE "/dev/cu.usbserial-A50178PJ"
#define _POSIX_SOURCE 1

#define FALSE 0
#define TRUE 1

using namespace std;

/*extern "C" {
int read(int handle, void *buf, unsigned n);
int write(int handle, void *buf, unsigned n);
}*/

class Serial {

public:
    int         fd, c, res;
    volatile int active;
	volatile int done;
    pthread_t   thread;
    struct      termios oldtio,newtio;
    char        buf[1024];
	int			rp, wp;
    char        device[128];
    vector<string> deviceList;
    
    void (*cb)(void *data, int size, void *user_data);
    void *ud;
    
    Serial();
    ~Serial();
    void threadStop();
    bool prepareSerial();
    void threadStart();
    void setDevice(const char *d);
    void setCallback(void (*callback)(void *data, int size, void *client), void *user_data);

    void writeData(void *data, int s);
    static void writeDataCallback(void *data, int s, void *user_data);

private:
    static void *threadFunction(void *data);
    
};

#endif
