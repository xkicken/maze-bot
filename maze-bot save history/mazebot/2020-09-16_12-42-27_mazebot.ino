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
        if (LineSensor.readSensors() < 3){ // stop if both sensor sees black
          m1.run(0);
          m2.run(0);
          Sequencestate = 3;
        }
        break;
      case 3:
        led.setColor(50, 0, 50); // turn right
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
      case 5:
        if(ultrasonic.distanceCm() > 7){
          Sequencestate = 1;
        }
        else{
          Sequencestate = 6;
        }
      case 6:
        m1.run(-150);
        m2.run(-150);
          if(LineSensor.readSensors() < 3){
            m1.stop();
            m2.stop();
          }
      break;
         }
}
