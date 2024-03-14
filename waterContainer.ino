#define sensorPower 7
#define sensorPin A0
int level = 0;
int quant = 0;
bool check = false;

//промени стойностите на променливите ако трябва

int val = 0;

void setup() {

  pinMode(sensorPower, OUTPUT);

  digitalWrite(sensorPower, LOW);
}

void loop() {

  level = readSensor();
  quant = level * 15 * 15;
  waterCap();
  // код за изпращане на количеството вода към сайта
  delay(1000);
}


int readSensor() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}

void waterCap() {
  if (quant < 100 && check == false) {
  //код за изпращане на съобщение до сайта: "Остава малко вода (под 100см)"
  check = true;
  }
  if (quant > 150) {
  check = false;
  }
}
