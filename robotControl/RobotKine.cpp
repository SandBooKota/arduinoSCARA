#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265359

#include "RobotParam.h"
#include "RobotKine.h"

//Constructer
CRobotKine::CRobotKine(){
  TargetDeg[4] = {0};
  TargetZ[2] = {0};
}

//Disconstructer
CRobotKine::~CRobotKine(){

}

int CRobotKine::InvKineScara(int RobotType,double x,double y,double z,double roll,double pitch){
  double l1,l2,l3;
  double A,B,C,D;
  int deg1,deg2,deg3;
  double th0,th1,th2,th3;
  double theta = y * PI/180;
  int Ha = -1;

  int z1=0,z2=0,sum=0;

  l1 = LINK1_Length;;
  l2 = LINK2_Length;
  if(RobotType == A4_TYPE){
    l3 = LINK3_Length;
    th3 = 0;
  }
  else if(RobotType == A5_TYPE){
    l3 = LINKW_Length + LINKP_Length*cos(pitch);
    z = z - LINKP_Length*sin(pitch);
    th3 = pitch;
  }

  //逆運動学
  A = y - l3*sin(theta);
  B = x - l3*cos(theta);
  C = (A*A + B*B + l1*l1 - l2*l2)/(2*l1);

  th0 = atan2(A, B) - atan2(Ha * sqrt(A*A+B*B-C*C), C);

  D = (A*A + B*B - l1*l1 + l2*l2)/(2*l2);

  th1 = atan2(Ha * sqrt(A*A+B*B-C*C), C) - atan2(-Ha * sqrt(A*A+B*B-D*D), D);
  th2 = theta - th0 - th1;

  TargetDeg[0] = th0*180/PI + MOTER1_InitAng + MOTER1_Offsset;
  TargetDeg[1] = th1*180/PI + MOTER1_InitAng + MOTER1_Offsset;
  TargetDeg[2] = th2*180/PI + MOTER1_InitAng + MOTER1_Offsset;
  TargetDeg[3] = th3*180/PI + MOTER1_InitAng + MOTER1_Offsset;

  if(z > 255){
    TargetZ[0] = 255;
    TargetZ[1] = z - 255;
  }
  else {
    TargetZ[0] = z;
    TargetZ[1] = 0;
  }
}
