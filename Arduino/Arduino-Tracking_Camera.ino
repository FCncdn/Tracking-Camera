#include <Wire.h>
#include <Servo.h>
#define SLAVE_ADDRESS 0x04
Servo servo_1;
Servo servo_2;
int Servo1Pin = 9;
int Servo2Pin = 8;
int servo1_angle;
int servo2_angle;
boolean servo_status;
int servo1_turn;
int servo2_turn;

void setup() {
Serial.begin(9600);
Wire.begin(SLAVE_ADDRESS);
Wire.onReceive(receiveData);
Wire.onRequest(sendData);
servo_1.attach(Servo1Pin);
servo_2.attach(Servo2Pin);
servo_status = false;
servo1_angle = 0;
servo2_angle = 140;
Serial.println("Ready!");
}

void loop() {
  if(!servo_status){
    servo_normal();
    }
  else{ 
    servo_control(); 
    }
}

void servo_normal(){
  Serial.println(servo_status);
  servo2_angle = 140;
  servo_2.write(servo2_angle);
  for (;servo1_angle <= 180; servo1_angle++) {
    if(servo_status){break;}
    servo_1.write(servo1_angle);
    delay(50);
  }
  for (; servo1_angle >= 0; servo1_angle--) {
    if(servo_status){break;}
    servo_1.write(servo1_angle);
    delay(50);
  }
  }

void servo_control(){
  while(1){
    Serial.print(servo2_turn);
    Serial.print(",");
    Serial.println(servo2_angle);
    if(servo1_turn >= 100 && servo1_angle >= 0){
      servo1_angle = servo1_angle-2;
      if (servo1_angle < 0){
        servo1_angle = 0;
      }
      servo_1.write(servo1_angle);
    }
    else if( servo1_turn <= 80 && servo1_angle <= 180){
      servo1_angle = servo1_angle+2;
      if (servo1_angle >180){
        servo1_angle = 180;
      }
      servo_1.write(servo1_angle);
    }
    if(servo2_turn <= 80 && servo2_angle >= 0){
      servo2_angle = servo2_angle+2;
      if (servo2_angle < 0){
        servo2_angle = 0;
      }
      servo_2.write(servo2_angle);
    }else if( servo2_turn >= 100 && servo2_angle <= 180){
      servo2_angle = servo2_angle-2;
      if (servo2_angle >180){
        servo2_angle = 180;
      }
      servo_2.write(servo2_angle);
    }
    delay(100);
    if(!servo_status){
      break;
    }
    }
  }

void receiveData(int byteCount){
  int cmd[4];
  int x = 0;
  int stat = 0;
  while(Wire.available()){
    cmd[x] = Wire.read();
    x++;
  }
  stat = cmd[1];  
  if (stat == 0){
    servo_status = false;  
  }else if(stat == 1){
    servo_status = true;
    servo1_turn = cmd[2];
    servo2_turn = cmd[3];
  }
}

void sendData(){
}
