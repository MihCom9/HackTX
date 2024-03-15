#define POWER_PIN  17
#define SIGNAL_PIN 25 // YOU MUST USE THIS PIN

int value = 0; 
int quant;

void setup() {
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT); 
  digitalWrite(POWER_PIN, LOW); 
}

void loop() {
  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  value = analogRead(SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW); 


  quant = (value/10) * 225;

  Serial.print("There are ");
  Serial.print(value);
  Serial.println("ml of water");
  
  delay(1000);
}





