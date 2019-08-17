#include <VarSpeedServo.h> 

VarSpeedServo myservo1;
VarSpeedServo myservo2;
VarSpeedServo myservo3;

const int val180_1=125;
const int val180_2=130;
const int val180_3=130;
//range=250[deg]

void setup() {
  myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(11);
  Serial.begin(9600);
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
      
      for(byte i = 0; i < 7; i++;){
        buf[i*2] = buf[i*2] << 4;
        val[i] = buf[i*2] | buf[i*2+1];
      }
    }
    
    myservo1.write(val[0]*val180_1/180,val[5],false);
    myservo2.write(val[1]*val180_2/180,val[5],false);
    myservo3.write(val[2]*val180_3/180,val[5],false);
    myservo1.wait();
    myservo2.wait();
    myservo3.wait();

    Serial.write('I');
  }
  
}
