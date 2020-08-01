#include <VarSpeedServo.h>
#include <Stepper.h>

const int val180_0 = 125;
const int val180_1 = 125;
const int val180_2 = 125;
const int val180_3 = 125;

VarSpeedServo myservo0;
VarSpeedServo myservo1;
VarSpeedServo myservo2;
VarSpeedServo myservo3;

const float turnSteps = 400;
float rpm = 30;
int value=1;

Stepper myStepper(turnSteps,3,4,5,6);
Stepper myRevStepper(turnSteps, 5,6,3,4);

int preDeg0, preDeg1, preDeg2, preDeg3, preZ;

void setup() {
  pinMode(48,INPUT);
  Serial.begin(9600);
  
  preDeg0 = 250;//190;
  preDeg1 = 10;
  preDeg2 = 150;
  preDeg3 = 125;
  preZ = 0;
  
  myservo0.attach(9);
  myservo0.write(preDeg0*val180_0/180,10,false);
  myservo0.wait();
  myservo1.attach(10);
  myservo1.write(preDeg1*val180_1/180,10,false);
  myservo1.wait();
  myservo2.attach(11); 
  myservo2.write(preDeg2*val180_2/180,10,false);
  myservo2.wait();
  myservo2.attach(12); 
  myservo2.write(preDeg3*val180_3/180,10,false);
  myservo2.wait();
  
  myStepper.setSpeed(rpm);
  myRevStepper.setSpeed(rpm);
  while(value==1){
    value = digitalRead(48);
    myRevStepper.step(1);
  }
  myStepper.step(10);
}

void loop() {
  int mode = 0;
  int val[7];
  int sum;
  
  if(Serial.available() >= sizeof('H') + sizeof(int)){
    int buf[15];
    if(Serial.read()=='H'){
      delay(50);
      for (byte i = 0 ; i < 15; i++){
        buf[i] = Serial.read();
        if(buf[i] >= '0' && buf[i] <= '9'){
          buf[i] = buf[i] - 0x30;
        }else{
          buf[i] = buf[i] - 0x40 + 9;
        }
      }

      for(byte i = 0; i < 7; i++){
        buf[i*2+1] = buf[i*2+1] << 4;
        val[i] = buf[i*2+1] | buf[i*2+2];
      }
    }

    mode = buf[0];

    //sum = val[0] + val[1] + val[2] + val[3] + val[4] + val[5];
    //sum = sum & 0xFF;
  
    servoControl(val[0],val[1],val[2],val[3],val[4]);
    zControl(val[5],val[6]);
    Serial.write('I'); 
  }
}

void servoControl(int deg0,int deg1,int deg2,int deg3,int sp){
  
  int diffDeg0, diffDeg1, diffDeg2, diffDeg3, diffDegSum;
  int sp0, sp1, sp2, sp3;
    
  diffDeg0 = abs(deg0 - preDeg0);
  diffDeg1 = abs(deg1 - preDeg1);
  diffDeg2 = abs(deg2 - preDeg2);
  diffDeg3 = abs(deg3 - preDeg3);

  diffDegSum = diffDeg0 + diffDeg1 + diffDeg2 + diffDeg3;

  sp0 = (diffDeg0/diffDegSum) * sp;
  sp1 = (diffDeg1/diffDegSum) * sp;
  sp2 = (diffDeg2/diffDegSum) * sp;
  sp3 = (diffDeg3/diffDegSum) * sp;

  myservo0.write(deg0*val180_0/180,sp0,false);
  myservo1.write(deg1*val180_1/180,sp1,false);
  myservo2.write(deg2*val180_2/180,sp2,false);
  myservo3.write(deg3*val180_3/180,sp2,false);

  preDeg0 = deg0;
  preDeg1 = deg1;
  preDeg2 = deg2;
  preDeg3 = deg3;

  myservo0.wait();
  myservo1.wait();
  myservo2.wait();
  myservo3.wait();
}

void zControl(int z1,int z2){
  int diffZ;
  float Steps;
  
  diffZ = z1 + z2 - preZ;
  Steps = 25 * abs(diffZ);
  
  if(diffZ < 0){
    myRevStepper.step(Steps);
  }else{
    myStepper.step(Steps);
  }
  preZ = z1 + z2; 
}
