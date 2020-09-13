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

void setup() {
led.setpin(13);
led.setColor(255, 255, 255);
led.show();
}

void loop() {
  led.setColor(0,0,0);
  led.show();
    switch(Sequencestate){
      case 0: //idle
        m1.run(0);
        m2.run(0);
        Sequencestate = 1;
        break;
      case 1:
        int sensorState = LineSensor.readSensors();
          switch(sensorState) //line following
          {
            case 0:
              m1.run(-255);
              m2.run(255);
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
              // Sequencestate = 2;
              break;
          }
          break;
      case 2: //intersection detection
        led.setColor(255, 255, 255); //Set both LED to White
        led.show();
        m1.run(-200);
        m2.run(200);
        if (LineSensor.readSensors() == 0){
          m1.run(0);
          m2.run(0);
          Sequencestate = 30;
        }
        break;
      case 3:
        m1.run(0);
        m2.run(200);
        if (LineSensor.readSensors() == 0){
          m1.run(0);
          m2.run(0);
        }
        break;
         }
}
