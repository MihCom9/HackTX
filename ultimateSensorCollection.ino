#include <PS4Controller.h>
#include <DRV8833.h>
#include <DHT.h>
#define DHTPIN 14
#define inputA1 13
#define inputA2 12
#define inputB1 26
#define inputB2 27
#define trigPin 16
#define echoPin 4
#define pumpPin 35
#define depthPower 15
#define depthPin 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
DRV8833 driver = DRV8833();

float duration, distance;
int level = 0;
int quant = 0;
bool check = false;
int val = 0;
int speed, steer;
int player = 1;
int battery = 0;

void setup() {
  Serial.begin(115200);
  PS4.begin();
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(depthPower, OUTPUT);
  digitalWrite(depthPower, LOW);
  dht.begin();
}

void loop() {
  DHT();
  wheels();
  ultra();
  plantCheck();
  level = readSensor();
  quant = (level/100) * 225;
  Serial.print("Ml: ");
  Serial.println(quant);
  waterCap();
}

void DHT() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: "); 
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  //ред за изпращане на стойностите към сайта
}
void wheels() {

  steer = 0;
  steer = (PS4.LStickX());

  speed = 0;
  speed = (PS4.LStickY());


  //STOP
  if ((speed < 30 && speed > -30) && (steer < 30 && steer > -30)) {
    driver.motorAForward(0);
    driver.motorAReverse(0);
    driver.motorBForward(0);
    driver.motorBReverse(0);
  }

  //GO BACK
  if (speed > 30 && steer < 30 && steer > -30) {
    driver.motorAReverse(speed * 2);
    driver.motorBReverse(speed * 2);
  }

  //GO FORWARD
  if (speed < -30 && steer < 30 && steer > -30) {
    driver.motorAForward(-speed * 2);
    driver.motorBForward(-speed * 2);
  }

  //GO LEFT BACK
  if (speed < 30 && steer > 30) {
    driver.motorAReverse(speed * 2);
    driver.motorBReverse(speed * 2 - steer * 2);
  }

  //GO RIGHT BACK
  if (speed < 30 && steer < -30) {
    driver.motorAReverse(speed * 2 + steer * 2);
    driver.motorBReverse(speed * 2);
  }

  //GO RIGHT FORWARD
  if (speed < -30 && steer > 30) {
    driver.motorAForward((-speed * 2) - steer * 2);
    driver.motorBForward(-speed * 2);
  }

  //GO LEFT FORWARD
  if (speed < -30 && steer < -30) {
    driver.motorAForward(-speed * 2);
    driver.motorBForward(-speed + steer * 2);
  }

  //SPIN RIGHT
  if (speed > -30 && speed < 30 && steer > 30) {
    driver.motorAReverse(0 + steer * 2);
    driver.motorBForward(0 + steer * 2);
  }

  //SPIN LEFT
  if (speed > -30 && speed < 30 && steer < -30) {
    driver.motorAForward(0 - steer * 2);
    driver.motorBReverse(0 - steer * 2);
  }

  delay(20);
}
void ultra() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Serial.print("Distance: ");
  Serial.println(distance);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
}
void plantCheck() {
  if (distance > 20) {
  } else {
    digitalWrite(pumpPin, HIGH);
    delay(100);
    digitalWrite(pumpPin, LOW);
    delay(100);
  }
}
int readSensor() {
  digitalWrite(depthPower, HIGH);
  delay(10);
  val = analogRead(depthPin);
  digitalWrite(depthPower, LOW);
  return val;
}
void waterCap() {
  if (quant < 100 && check == false) {
  Serial.println("Остава малко вода (под 100см)");
  check = true;
  }
  if (quant > 150) {
  check = false;
  }
}
