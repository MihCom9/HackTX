#include <PS4Controller.h>
#include <DRV8833.h>

DRV8833 driver = DRV8833();

#define inputA1 13
#define inputA2 12
#define inputB1 26
#define inputB2 27

int forward, back, steer;
int forward_value, back_value, steer_value;
int player = 1;
int battery = 0;
int motorspeedf, motorspeedb;

void setup()
{
  Serial.begin(115200);
  PS4.begin();
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  Serial.println("Ready!");
}

void loop()
{

  steer = 0;
  steer = (PS4.LStickX());

  motorspeedb = 0;
  motorspeedb =  (PS4.L2Value());
  
  motorspeedf = 0;
  motorspeedf = (PS4.R2Value());
  
  if((PS4.L2Value())>0 && (PS4.R2Value())==0 && steer<10 && steer>-10)
  {
    driver.motorAReverse(motorspeedb);
    driver.motorBReverse(motorspeedb);
  }

  if((PS4.R2Value())>0 && (PS4.L2Value())==0 && steer<10 && steer>-10)
  {
    driver.motorAForward(motorspeedf);
    driver.motorBForward(motorspeedf);
  }
  
  if((PS4.R2Value())>0 && (PS4.L2Value())>0 && steer<10 && steer>-10)
  {
  driver.motorAForward(motorspeedf);
  driver.motorBForward(motorspeedf);
  }

  if((PS4.R2Value())==0 && (PS4.L2Value())==0 && steer<10 && steer>-10)
  {
    driver.motorAForward(0);
    driver.motorAReverse(0);
    driver.motorBForward(0);
    driver.motorBReverse(0);
  }

  if((PS4.L2Value())>0 && (PS4.R2Value())==0 && steer>10)
  {
    driver.motorAReverse(motorspeedb);
    driver.motorBReverse(motorspeedb-steer*2);
  }

  if((PS4.L2Value())>0 && (PS4.R2Value())==0 && steer<10)
  {
    driver.motorAReverse(motorspeedb+steer*2);
    driver.motorBReverse(motorspeedb);
  }

  if((PS4.R2Value())>0 && (PS4.L2Value())==0 && steer>10)
  {
    driver.motorAForward(motorspeedf-steer*2);
    driver.motorBForward(motorspeedf);
  }

  if((PS4.R2Value())>0 && (PS4.L2Value())==0 && steer<-10)
  {
    driver.motorAForward(motorspeedf);
    driver.motorBForward(motorspeedf+steer*2);
  }
  
  if((PS4.R2Value())==0 && (PS4.L2Value())==0 && steer>10)
  {
    driver.motorAReverse(motorspeedf+steer*2);
    driver.motorBForward(motorspeedf+steer*2);
  }

  
  if((PS4.R2Value())==0 && (PS4.L2Value())==0 && steer<-10)
  {
    driver.motorAForward(motorspeedf-steer*2);
    driver.motorBReverse(motorspeedf-steer*2);
  }

  delay(20);

}

