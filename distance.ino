float speed, distance;
float distanceSum = 0;
int sliderValue;

void setup() {
}

void loop() {
  //кода за взимане на стойността на слайдера от сайта
  speed = sliderValue*0.03;
  distance = (speed*3600) * 0.02;
  distanceSum = distanceSum + distance;
  distanceReset();
  delay(20);
  //код за пращане на изминатата дистанция на бота
}
void distanceReset() {
  if(/*бутона в сайта е натиснат*/){
    distanceSum = 0;
  }
}
