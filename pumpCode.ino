int relayPin = 2;
int pumpPin = 13;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);
}

void loop() {
  digitalWrite(relayPin, HIGH);
  delay(100);
  digitalWrite(relayPin, LOW);
  delay(100);
}