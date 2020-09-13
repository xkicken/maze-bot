#include <MeMCore.h>

//Motors
MeDCMotor m1(M1); //left motor
MeDCMotor m2(M2); //right motor

//Sensors
MeUltrasonicSensor ultrasonic(PORT_3);
MeLineFollower LineSensor(PORT_2);

int Sequencestate = 1;

void setup() {

}

void loop() {
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
              m1.run(-150);
              m2.run(150);
              break;
            case 1:
              m1.run(0);
              m2.run(150);
              break;
            case 2:
              m1.run(-150);
              m2.run(0);
              break;
            case 3:
              Sequencestate = 2;
              break;
          }
          break;
      case 2: //intersection detection
        while (LineSensor.readSensors() == 0){
          m1.run(-200);
          m2.run(200);
        }
        Sequencestate = 3;
        break;
      case 3:
        while(LineSensor.readSensors() == 0){
          m1.run(0);
          m1.run(200);
        }
        Sequencestate = 4;
        break;
         }
}
