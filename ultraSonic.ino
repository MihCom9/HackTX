const int trigPin = 17;
const int echoPin = 16;
float duration, distance;

//промени стойностите на пиновете

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  ultra();
  plantCheck();
  delay(50);
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
    //код за спиране на помпата
  } else {
    //код за пускане на помпата
  }
}