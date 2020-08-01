#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <math.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>

#define PI 3.14159265359

#include "arduino-serial-lib.h"
#include "RobotCon.h"
#include "RobotKine.h"
#include "RobotParam.h"
#include "CommParam.h"

int offset1=5,offset2=0,offset3=4;

int CRobotCon::AngleWrite(int mode,int deg1,int deg2,int deg3,int z,int sp){
    int fd = -1;
    char serialport[256] = "/dev/ttyACM0";
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    int rc,n;

    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) printf("couldn't open port");
    if(!quiet) printf("opened port %s\n",serialport);
    serialport_flush(fd);
    if( fd == -1 ) printf("serial port not opened");

    char buf[256];
    int sum;
    int z1, z2;

    deg1 = deg1 + MOTER1_Offsset;
    deg2 = deg2 + MOTER2_Offsset;
    deg3 = deg3 + MOTER3_Offsset;

    if(z > 255){
      z1 = 255;
      z2 = z - 255;
    }
    else {
      z1 = z;
      z2 = 0;
    }

  	snprintf(buf, 30, "H%01X%02X%02X%02X%02X%02X%02X%02X",mode, deg1, deg2, deg3, z1, z2, sp, sum);
    if( !quiet ) printf("send string:%s\n", buf);
  	rc = serialport_write(fd, buf);
    if(rc==-1) printf("error writing");

  	while(1){
  		if( fd == -1 ) printf("serial port not opened");
  		memset(buf,0,1);
  		serialport_read_until(fd, buf, eolchar, 1, timeout);
  		if( !quiet ) printf("read string:");
  		if(!strcmp(buf, "I")){
  			printf("Reach target\n");
  		break;
  		}
    }

}

int CRobotCon::CoordMove(int mode,double x,double y,double z,double roll,double pitch,int sp){
  int fd = -1;
  char serialport[256] = "/dev/ttyACM0";
  int baudrate = 9600;
  char quiet=0;
  char eolchar = '\n';
  int timeout = 5000;
  int rc,n;
  char buf[256];
  int MoveFlag = 0;
  int deg[4], zr[2], sum;

  CRobotKine scaraKine;

  scaraKine.InvKineScara(A5_TYPE,x,y,z,roll,pitch);

  for(int i=0; i < 4; i++){
    if(scaraKine.TargetDeg[i] > MOTER_AngleMax
       || scaraKine.TargetDeg[i] < MOTER_AngleMin){
      printf("Servo%d Limit over\nTarget Angle %d[deg]\n",i+1,scaraKine.TargetDeg[i]);
      MoveFlag = 1;
    }
    else{
      deg[i] = scaraKine.TargetDeg[i];
    }
  }

  if((scaraKine.TargetZ[0]+scaraKine.TargetZ[1]) > LNEARZ_Max
  || (scaraKine.TargetZ[0]+scaraKine.TargetZ[1]) < LNEARZ_Min){
    printf("Z Linear Limit over\nTarget Z %d[mm]\n",scaraKine.TargetZ[0]+scaraKine.TargetZ[1]);
    MoveFlag = 1;
  }
  else{
    zr[0] = scaraKine.TargetZ[0];
    zr[1] = scaraKine.TargetZ[1];
  }

  if(MoveFlag == 0){
    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) printf("couldn't open port");
    if(!quiet) printf("opened port %s\n",serialport);
    serialport_flush(fd);
    if( fd == -1 ) printf("serial port not opened");

    snprintf(buf, 30, "H%01X%02X%02X%02X%02X%02X%02X%02X", mode, deg[0], deg[1], deg[2], deg[3], sp, zr[0], zr[1]);
    if( !quiet ) printf(" send string:%s\n", buf);
    rc = serialport_write(fd, buf);
    if(rc==-1) printf("error writing");

  	//目標点到達：Arduino側から文字”I”が送信
  	while(1){
  		if( fd == -1 ) printf("serial port not opened");
  		memset(buf,0,1);
  		serialport_read_until(fd, buf, eolchar, 1, timeout);
  		if( !quiet ) printf(" read string:");
  		if(!strcmp(buf, "I")){
  			printf("Reach target\n");
  		break;
  		}
    }
  }

}
