#include <VarSpeedServo.h> 

VarSpeedServo myservo0;
VarSpeedServo myservo1;
VarSpeedServo myservo2;

const int val180_0 = 125;
const int val180_1 = 130;
const int val180_2 = 130;
//range=250[deg]

int preDeg0, preDeg1, preDeg2;

void setup() {
  myservo0.attach(9);
  myservo1.attach(10);
  myservo2.attach(11);
  Serial.begin(9600);
  preDeg0 = 190;
  preDeg1 = 0;
  preDeg2 = 185; 
}


void loop() {
  if(Serial.available() >= sizeof('H') + sizeof(int)){

    int val[7];
    int buf[14];
    
    if(Serial.read()=='H'){
      delay(50);
      
      for (byte i = 0 ; i < 14; i++){
        buf[i] = Serial.read();
        if(buf[i] >= '0' && buf[i] <= '9'){
          buf[i] = buf[i] - 0x30;
        }else{
          buf[i] = buf[i] - 0x40 + 9;
        }
      }
      
      for(byte i = 0; i < 7; i++){
        buf[i*2] = buf[i*2] << 4;
        val[i] = buf[i*2] | buf[i*2+1];
      }
    }
    
    int diffDeg0, diffDeg1, diffDeg2, sp0, sp1, sp2;
    
    diffDeg0 = val[0] - preDeg0;
    diffDeg1 = val[1] - preDeg1;
    diffDeg2 = val[2] - preDeg2;
    
    sp0 = abs(diffDeg0/(diffDeg0 + diffDeg1 + diffDeg2)) * val[5];
    sp1 = abs(diffDeg1/(diffDeg0 + diffDeg1 + diffDeg2)) * val[5];
    sp2 = abs(diffDeg2/(diffDeg0 + diffDeg1 + diffDeg2)) * val[5];
    
    myservo0.write(val[0]*val180_0/180,sp0,false);
    myservo1.write(val[1]*val180_1/180,sp1,false);
    myservo2.write(val[2]*val180_2/180,sp2,false);
    myservo0.wait();
    myservo1.wait();
    myservo2.wait();

    Serial.write('I');
  }
  
}
