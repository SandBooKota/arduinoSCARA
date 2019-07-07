/*
 * arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 *   gcc -o arduino-serial arduino-serial-lib.c arduino-serial.c
 * or use the included Makefile
 *
 * Mac: make sure you have Xcode installed
 * Windows: try MinGW to get GCC
 *
 *
 * Originally created 5 December 2006
 * 2006-2013, Tod E. Kurt, http://todbot.com/blog/
 *
 *
 * Updated 8 December 2006:
 *  Justin McBride discovered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 * Update 6 April 2012:
 *  Split into a library and app parts, put on github
 *
 * Update 20 Apr 2013:
 *  Small updates to deal with flushing and read backs
 *  Fixed re-opens
 *  Added --flush option
 *  Added --sendline option
 *  Added --eolchar option
 *  Added --timeout option
 *  Added -q/-quiet option
 *
 */

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <math.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>

#define PI 3.14159265359

#include "arduino-serial-lib.h"
void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int servoWrite(int fd,int deg1,int deg2,int sp);
int coordinateWrite(int fd,double x,double y,int sp);

int pre_deg;

int main(int argc, char *argv[])
{
    int fd = -1;
    char serialport[256] = "/dev/ttyACM0";
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    int rc,n;

    double x,y;


    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) error("couldn't open port");
	if(!quiet) printf("opened port %s\n",serialport);
	serialport_flush(fd);

    if( fd == -1 ) error("serial port not opened");

    int testSpeed=10;

	servoWrite(fd,250,0,testSpeed);

	coordinateWrite(fd,1,120,testSpeed);
	
	coordinateWrite(fd,1,250,testSpeed);
	
    exit(EXIT_SUCCESS);
} // end main

int servoWrite(int fd,int deg1,int deg2,int sp){
    char buf[256];
    char quiet=0;
    int rc;

	snprintf(buf, 12, "H%02X%02X%02X", deg1, deg2, sp);
    if( !quiet ) printf("send string:%s\n", buf);
	rc = serialport_write(fd, buf);
    if(rc==-1) error("error writing");

	char eolchar = '\n';
    int timeout = 5000;

	while(1){
		if( fd == -1 ) error("serial port not opened");
		memset(buf,0,1);
		serialport_read_until(fd, buf, eolchar, 1, timeout);
		if( !quiet ) printf("read string:");
		if(!strcmp(buf, "I")){
			printf("Reach target\n");
		break;
		}
    }

}

int coordinateWrite(int fd,double x,double y,int sp){
    char buf[256];
    char quiet=0;
    int rc;

    double l1=127,l2=127;
    double A,B,C,D;
    int deg1,deg2;
    double th1,th2;

    //逆運動学
	A = pow(x,2)+pow(y,2)-pow(l1,2)-pow(l2,2);
	B = sqrt( 4 - ( pow(A,2)/(pow(l1,2)*pow(l2,2)) ));
	C = l1 + A/(2*l1);
	th1 = atan(y/x) - atan((l2*B)/(2*C));

	D = A/(2*l1*l2);
	th2 = acos(D);

	//printf("th1:%f th2:%f\n",th1,th2);
	deg1=215-th1*180/PI;
	deg2=125-th2*180/PI;
	printf("%.2f deg1:%d deg2:%d\n",y,deg1,deg2);

	snprintf(buf, 12, "H%02X%02X%02X", deg1, deg2, sp);
    if( !quiet ) printf("send string:%s\n", buf);
	rc = serialport_write(fd, buf);
    if(rc==-1) error("error writing");

	char eolchar = '\n';
    int timeout = 5000;

	//目標点到達時、Arduino側から文字”I”が送信
	while(1){
		if( fd == -1 ) error("serial port not opened");
		memset(buf,0,1);
		serialport_read_until(fd, buf, eolchar, 1, timeout);
		if( !quiet ) printf("read string:");
		if(!strcmp(buf, "I")){
			printf("Reach target\n");
		break;
		}
    }
}
