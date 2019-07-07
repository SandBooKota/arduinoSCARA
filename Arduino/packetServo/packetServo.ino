#include <VarSpeedServo.h> 

VarSpeedServo myservo1;
VarSpeedServo myservo2;

const int val180_1=125;
const int val180_2=130;
//range=250[deg]

void setup() {
  myservo1.attach(9);
  myservo2.attach(10);
  Serial.begin(9600);
}


void loop() {
  if(Serial.available() >= sizeof('H') + sizeof(int)){

    int val[3];
    int buf[6];
    
    if(Serial.read()=='H'){
      delay(50);
      
      for (byte i = 0 ; i < 6; i++){
        buf[i] = Serial.read();
        if(buf[i] >= '0' && buf[i] <= '9'){
          buf[i] = buf[i] - 0x30;
        }else{
          buf[i] = buf[i] - 0x40 + 9;
        }
      }
      buf[0] = buf[0] << 4;
      buf[2] = buf[2] << 4;
      buf[4] = buf[4] << 4;

      val[0] = buf[0] | buf[1];
      val[1] = buf[2] | buf[3];
      val[2] = buf[4] | buf[5];
    }
    
    myservo1.write(val[0]*val180_1/180,val[2],false);
    myservo2.write(val[1]*val180_2/180,val[2],false);
    myservo1.wait();
    myservo2.wait();
  }
  
}
