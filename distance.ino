#include <PS4Controller.h>
#include <DRV8833.h>

float speed, distance;
float distanceSum = 0;

void setup() {
  PS4.begin();
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
}

void loop() {

  speed = (PS4.LStickY()) * 0.787401575; //промени PS4.LStickY с дйоистика от сайта
  distance = (speed*3600) * 0.02;
  distanceSum = distanceSum + distance;
  distanceReset();
  delay(20)
  //код за пращане на изминатата дистанция на бота
}
void distanceReset() {
  //if statement за копче в сайта, което да прави distance = 0
}
