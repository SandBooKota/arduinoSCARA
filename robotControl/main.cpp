#include "RobotCon.h"
#include <unistd.h>

int main(){
  CRobotCon RobotCon;
  //RobotCon.CoordMove(0,x,y,z,roll,pitch,sp);
  for(int i=0; i<3; i++){
    RobotCon.CoordMove( 1, 0.0001, 160, 0, 90, 0,100);
    sleep(1);
    RobotCon.CoordMove( 1, 0.0001, 440, 0, 90, 0,100);
    sleep(1);
  }
}
