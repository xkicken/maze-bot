#include <MeMCore.h>

//Motors
MeDCMotor m1(M1); //left motor
MeDCMotor m2(M2); //right motor

//Sensors
MeUltrasonicSensor ultrasonic(PORT_3);
MeLineFollower LineSensor(PORT_2);

//other devices
MeRGBLed led(0, 30);

int Sequencestate = 0;
int sensorState;

void setup() {
led.setpin(13);
led.setColor(255, 255, 255);
led.show();
Serial.begin(9600);
}

void loop() {
  Serial.println(Sequencestate);
  Serial.println(ultrasonic.distanceCm());
  sensorState = LineSensor.readSensors();
  led.setColor(0,0,0);
  led.show();
    switch(Sequencestate){
      case 0: //idle
        m1.run(0);
        m2.run(0);
        Sequencestate = 1;
        break;
      case 1:
          switch(sensorState) //line following
          {
            case 0:
              m1.run(-150);
              m2.run(150);
              break;
            case 1:
              m1.run(0);
              m2.run(150);
              led.setColor(0, 0, 255);
              led.show();
              break;
            case 2:
              m1.run(-150);
              m2.run(0);
              led.setColor(0, 255, 0);
              led.show();
              break;
            case 3:
              m1.run(0);
              m2.run(0);
              led.setColor(255, 0, 0);
              led.show();
              Sequencestate = 2;
              break;
          }
      break;
      case 2: //intersection detection
        led.setColor(50, 50, 0); //Set both LED to White
        led.show();
        m1.run(-200);
        m2.run(200);
        if (LineSensor.readSensors() < 3){ // 1 or more sensor sees black
          m1.run(0);
          m2.run(0);
          Sequencestate = 3;
        }
      break;
      case 3:
        led.setColor(50, 0, 50); // turn left
        led.show();
        m1.run(150);
        m2.run(150);
        if (LineSensor.readSensors() == 2){
        Sequencestate = 4;
        }
        break;
      case 4:
        led.setColor(0, 50, 50);
        led.show();
        m1.run(150);
        m2.run(150);
        if (LineSensor.readSensors() == 0){ //turn untill both sensor is black
          m1.run(0);
          m2.run(0);
          Sequencestate = 5;
        }
      break;
      case 5: //check distance if bigger than 7 go case 1 else case 6
        if(ultrasonic.distanceCm() > 20){
          Sequencestate = 1;
        }
        else{
          Sequencestate = 6;
        }
      break;
      case 6: //turn right when sensor 2 sees white go to case 7
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() == 1){
            Sequencestate = 7;
          }
      break;
      case 7://turn right untill both sensor sees black when switch to case 8
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() == 0){
            m1.stop();
            m2.stop();
            Sequencestate = 8;
          }
      break;
      case 8: //check distance if larger than 7cm than
        if(ultrasonic.distanceCm() > 20){
          Sequencestate = 1;
        }
        else{
          Sequencestate = 9;
        }
      break;
      case 9: //turn right when sensor sees white switch to case 10
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() == 1){
            Sequencestate = 10;
          }
      break;
      case 10: //turn right untill both sensor sees black
        m1.run(-150);;
        m2.run(-150);
          if(LineSensor.readSensors() == 0){
            m1.stop();
            m2.stop();
            Sequencestate = 11;
          }
      break;
      case 11:
        if(ultrasonic.distanceCm() >20){ //check distance if greater than 20 switch case 1 else switch case 12
          Sequencestate = 1;
        }
        else{
          Sequencestate = 12;
        }
      break;
      case 12:
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() == 1){
            Sequencestate = 13;
          }
      break;
      case 13:
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() == 0){
            m1.stop();
            m2.stop();
            Sequencestate = 1;
          }
      break;
      }
  }
