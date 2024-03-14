#define PumpOn G19
#define FiveVoltRaley G15
#define PinPump G4 
void setup() {
  // put your setup code here, to run once:
  pinMode(PinPum,OUTPUT);
  pinMode(FiveVoltRaley,OUTPUT);
  pinMode(PumpOn,OUTPUT);
  digitalWriteline(FiveVoltRaley,HIGH);
  digitalWriteline(PumpOn,HIGH);
}

void loop() {
  digitalWriteline(PinPump,HIGH);
  delay(100);
  digitalWriteline(PinPump,HIGH);
  delay(100);
}
