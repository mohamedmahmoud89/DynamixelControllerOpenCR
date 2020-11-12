#include "controller.hpp"

Controller ctrl;

void setup() {
  // put your setup code here, to run once:
  ctrl.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(57600);
  while(!Serial); // Wait for Opening Serial Monitor
  Serial.available()>0?ctrl.Update(Serial.read()):ctrl.Update();
  delay(50);
}
