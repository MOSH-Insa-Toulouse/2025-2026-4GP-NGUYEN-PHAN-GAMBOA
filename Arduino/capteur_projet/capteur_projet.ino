#include "menu_OLED.h"
#include "digipot.h"

#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 7
SoftwareSerial mySerial(rxPin, txPin);


const float VCC = 5.0;      // voltage at Ardunio 5V line

// Servomotor
#include <Servo.h>
Servo myservo;


// Declare variables for the graphite sensor
const int graphitePin = A0; 
const float R1 = 100000.0;
const float R3 = 100000.0;
const float R5 = 10000.0;

// Declare variables for the flex sensor
const int flexPin = A1;      // Pin connected to voltage divider output
const float R_DIV = 27000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0;  // resistance at 90 deg

unsigned long lastSend = 0;
const unsigned long interval = 100; // 100 ms

static char selected_sensor = ' ';

void setup() {
  Serial.begin(9600);
  pinMode(flexPin, INPUT);
  pinMode(graphitePin, INPUT);

  initDigipot();
  initMenu();

  //Bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  //Servomotor
  myservo.attach(9);

  mySerial.begin(9600);

}
//Servomotor
void applyAngle(int angle) {
  myservo.write(angle);
}
void loop() {
  updateMenu();
  delay(5);
  
  if (Serial.available() > 0 || mySerial.available()) {
    char cmd;
    if (Serial.available() > 0) {
      cmd = Serial.read();
      if (cmd == '\n' || cmd == '\r') {
    return;
  }
    }
    else if (mySerial.available()) {
      cmd = mySerial.read();
    }
    
    Serial.print("MIT sends ");
    Serial.println(cmd);
    if (cmd == 'C') {
      selected_sensor = 'C'; // *************
    }
    else if (cmd == 'G') {
      selected_sensor = 'G'; // graphite sensor***************
    }
    else if (cmd == 'F') {
      selected_sensor = 'F'; // flex sensor***************
    }
    else if (cmd == 'A') {
      selected_sensor = 'A';
    }
    else {
      selected_sensor = ' ';
    }
  
  }
  // Send data according to the selected sensor
  if (millis() - lastSend >= interval) {
      lastSend = millis();
      if (selected_sensor == 'G' || selected_sensor == 'C') {
        int ADCgraphite = analogRead(graphitePin);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
        float Vgraphite = ADCgraphite * VCC / 1023.0;
        //R2 = 50000.0;
        float Rgraphite = (1+R3/R2)*R1*(VCC/Vgraphite)-R1-R5;
        mySerial.println(Rgraphite);
        Serial.println(Vgraphite);
        Serial.println(R2);
        Serial.println(Rgraphite);
        if (selected_sensor == 'G') {
          selected_sensor = ' ';
    }
  }
      else if (selected_sensor == 'F') {
        int ADCflex = analogRead(flexPin);
        float Vflex = ADCflex * VCC / 1023.0;
        float Rflex = R_DIV * (VCC / Vflex - 1.0);
        mySerial.println(Rflex);
        Serial.println(Rflex);
        selected_sensor = ' ';
  }
      else if (selected_sensor == 'A') {
        int ADCflex = analogRead(flexPin);
        float Vflex = ADCflex * VCC / 1023.0;
        float Rflex = R_DIV * (VCC / Vflex - 1.0);
        mySerial.println(Rflex);
        Serial.println(Rflex);
        
  }

}
}