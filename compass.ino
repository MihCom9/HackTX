
#include <Wire.h>

#include <LSM303.h>

LSM303 cmps;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  cmps.init();
  cmps.enableDefault();

  Serial.println("Setup complete...");
  
  cmps.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  cmps.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void loop() {
  cmps.read();
  
  float heading = cmps.heading();

  Serial.print("Heading:  ");
  Serial.println(heading);
  delay(200);
}
