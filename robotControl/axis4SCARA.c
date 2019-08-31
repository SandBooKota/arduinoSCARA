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

int servoWrite(int fd,int deg1,int deg2,int deg3,int z1, int z2,int sp);
int coordinateWrite(int fd,double x,double y,double z,double deg,int sp);

int offset1=2,offset2=1,offset3=-2;

int main(int argc, char *argv[])
{
  int fd = -1;
  char serialport[256] = "/dev/ttyACM0";
  int baudrate = 9600;  // default
  char quiet=0;
  char eolchar = '\n';
  int timeout = 5000;
  int rc,n;

  fd = serialport_init(serialport, baudrate);
  if( fd==-1 ) error("couldn't open port");
  if(!quiet) printf("opened port %s\n",serialport);
  serialport_flush(fd);

  if( fd == -1 ) error("serial port not opened");

  int servoSpeed = 50;

  //servoWrite(fd,127,126,123,0,0,testSpeed);

  int x,y;

  y=190;
  for(int i = 0; i <= 20; i++){
    coordinateWrite(fd,1,y,0,90,servoSpeed);
    y += 10;
  }

  exit(EXIT_SUCCESS);
} // end main

int servoWrite(int fd,int deg1,int deg2,int deg3,int z1, int z2,int sp){
    char buf[256];
    char quiet=0;
    int rc;
    int sum;

	snprintf(buf, 28, "H%02X%02X%02X%02X%02X%02X%02X", deg1+offset1, deg2+offset2, deg3+offset3, z1, z2, sp, sum);
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

int coordinateWrite(int fd,double x,double y,double z,double deg,int sp){
  char buf[256];
  char quiet=0;
  int rc;

  double l1=168,l2=168,l3=40;
  double A,B,C,D;
  int deg1,deg2,deg3;
  double th1,th2,th3;
	double theta = deg * PI/180;

	int z1=0,z2=0,sum=0;

    //逆運動学
	A = y - l3*sin(theta);
	B = x - l3*cos(theta);
	C = (A*A + B*B + l1*l1 - l2*l2)/(2*l1);
	th1 = atan2(A, B) - atan2(+sqrt(A*A+B*B-C*C), C);

	D = (A*A + B*B - l1*l1 + l2*l2)/(2*l2);
	th2 = atan2(+sqrt(A*A+B*B-C*C), C) - atan2(-sqrt(A*A+B*B-D*D), D);

	th3 = theta - th1 - th2;

  printf("deg1:%.4f deg2:%.4f deg3:%.4f\n",th1*180/PI,th2*180/PI,th3*180/PI);

  deg1 = 215 - th1*180/PI + offset1;
  deg2 = 125 - th2*180/PI + offset2;
  deg3 = 125  - th3*180/PI + offset3;
  //printf("deg1:%d deg2:%d deg3:%d\n",deg1,deg2,deg3);

  if(deg1 >= 250 || deg2 >= 250 || deg3 >= 250 || deg1 <= 0 || deg2 <= 0 || deg3 <= 0
  ||  sqrt(pow(x,2)+pow(y,2)) >= 376){
    printf("Limit over\n");
  }
  else{
    snprintf(buf, 28, "H%02X%02X%02X%02X%02X%02X%02X", deg1, deg2, deg3, z1, z2, sp, sum);
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
}
