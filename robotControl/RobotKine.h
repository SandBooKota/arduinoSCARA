#ifndef __ROBOT_KINE_H__
#define __ROBOT_KINE_H__

class CRobotKine{
public:
  CRobotKine();
  ~CRobotKine();
  int InvKineScara(int RobotType,double x,double y,double z,double roll,double pitch);
  int TargetDeg[4];
  int TargetZ[2];
};

#endif
