#ifndef _ROBOT_CON_H__
#define _ROBOT_CON_H__

class CRobotCon{
public:
  int AngleWrite(int mode,int deg1,int deg2,int deg3,int z,int sp);
  int CoordMove(int mode,double x,double y,double z,double roll,double pitch,int sp);
};

#endif
