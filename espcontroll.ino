#include <PS4Controller.h>
#include <DRV8833.h>

DRV8833 driver = DRV8833();

#define inputA1 13
#define inputA2 12
#define inputB1 26
#define inputB2 27

int speed, steer;
int player = 1;
int battery = 0;

void setup()
{
  Serial.begin(115200);
  PS4.begin();
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
}

void loop()
{

  steer = 0;
  steer = (PS4.LStickX());

  speed = 0;
  speed = (PS4.LStickY());


  //STOP
  if((speed<30 && speed>-30) && (steer<30 && steer>-30))
  {
    driver.motorAForward(0);
    driver.motorAReverse(0);
    driver.motorBForward(0);
    driver.motorBReverse(0);
  }

  //GO BACK
  if(speed>30 && steer<30 && steer>-30)
  {
    driver.motorAReverse(speed*2);
    driver.motorBReverse(speed*2);
  }

  //GO FORWARD
  if(speed<-30 && steer<30 && steer>-30)
  {
    driver.motorAForward(-speed*2);
    driver.motorBForward(-speed*2);
  }

  //GO LEFT BACK
  if(speed<30 && steer>30)
  {
    driver.motorAReverse(speed*2);
    driver.motorBReverse(speed*2-steer*2);
  }

  //GO RIGHT BACK
  if(speed<30 && steer<-30)
  {
    driver.motorAReverse(speed*2+steer*2);
    driver.motorBReverse(speed*2);
  }

  //GO RIGHT FORWARD
  if(speed<-30 && steer>30)
  {
    driver.motorAForward((-speed*2)-steer*2);
    driver.motorBForward(-speed*2);
  }

  //GO LEFT FORWARD
  if(speed<-30 && steer<-30)
  {
    driver.motorAForward(-speed*2);
    driver.motorBForward(-speed+steer*2);
  }
  
  //SPIN RIGHT
  if(speed>-30 && speed<30 && steer>30)
  {
    driver.motorAReverse(0+steer*2);
    driver.motorBForward(0+steer*2);
  }

    //SPIN LEFT
  if(speed>-30 && speed<30 && steer<-30)
  {
    driver.motorAForward(0-steer*2);
    driver.motorBReverse(0-steer*2);
  }

  delay(20);

}

