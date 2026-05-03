#include "menu_OLED.h"
#include "digipot.h"

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(-1);

#define encoderPinA 2   // CLK
#define encoderPinB 4   // DT
#define encoderButton 3  // SW

volatile int encoderPos = 1;

volatile int mainMenuCount = 1;
volatile int servoMenuCount = 1;
volatile int angle = 0;

void applyAngle(int angle);

static void doEncoder();
static void drawMainMenu();
static void drawSubMenuGain();
static void handleButton();
static void drawSubMenuServoMorto();

volatile bool menuChanged = true;
volatile bool isChoosingAngle = false;
volatile bool isApplyingAngle = false;

unsigned long lastClickTime = 0;

enum MenuLevel {
  MAIN_MENU,
  SUB_MENU_GAIN,
  SUB_MENU_SERVOMOTOR
};

volatile MenuLevel menuLevel = MAIN_MENU;

byte lastButtonState = HIGH;

void initMenu() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderButton, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();

  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoder, RISING);
  applyGain(128);

}

static void doEncoder() {
  if (menuLevel == MAIN_MENU) {
    if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==HIGH) {
      if (encoderPos < 2){
        encoderPos++;
        Serial.println(encoderPos);
    }
  } else if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==LOW) {
      if (encoderPos > 1){
        encoderPos--;
        Serial.println(encoderPos);
    }
  }
  }
  else if (menuLevel == SUB_MENU_GAIN) {
    if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==LOW) {
      if (encoderPos < 128){
        encoderPos++;
    }
  } else if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==HIGH) {
      if (encoderPos > 0){
        encoderPos--;
    }
  }
  }
  else {
    if (isChoosingAngle == true){
      if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==HIGH) {
        if (angle < 180){
          angle+=5;
    }
  }   else if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==LOW) {
        if (angle > 0){
          angle-=5;
    }
  }
    }
    else {
      if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==HIGH) {
        if (encoderPos < 2){
           encoderPos++;
    }
  }   else if (digitalRead(encoderPinA)==HIGH && digitalRead(encoderPinB)==LOW) {
        if (encoderPos > 1){
          encoderPos--;
    }
  }
    }
    
  } 

  menuChanged = true; 
  
}

void updateMenu() {
  handleButton();
  if(menuChanged) {
    menuChanged = false;
    display.clearDisplay();
    if (menuLevel == MAIN_MENU) {
      drawMainMenu();
    }
    else if (menuLevel == SUB_MENU_GAIN){
      drawSubMenuGain(); 
    }
    else {
      drawSubMenuServoMorto();
    }
  }  
  display.display();
}

static void drawMainMenu() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("PROJET CAPTEUR");

  display.setCursor(10, 10);
  display.println("Adjust Gain Level");
  
  display.setCursor(10, 20);
  display.println("Servo Motor");
 
  display.setCursor(0, encoderPos*10);
  display.println(">");
}

static void drawSubMenuGain() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("ADJUST GAIN LEVEL");

  display.drawRect(0, 10, 128, 10, WHITE);
  display.fillRect(0, 10, 128-encoderPos, 10, WHITE);
  applyGain(encoderPos);

  display.setCursor(0, 25);
  display.println(">");
  display.setCursor(10, 25);
  display.println("BACK");


}

static void drawSubMenuServoMorto() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  if (isApplyingAngle == true) {
    display.println("APPLY an angle:");
    isApplyingAngle == false;
  }
  else if (isChoosingAngle == false) {
    display.println("Choose an angle:");
  }
  else if (isChoosingAngle == true) {
    display.println("CHOOSE AN ANGLE:");
  }
  display.setCursor(110, 0);
  display.println(angle);

  display.setCursor(10, 10);
  display.println("BACK");

  display.setCursor(0, encoderPos*10-10);
  display.println(">");
}

static void handleButton() {
  byte currentButtonState = digitalRead(encoderButton);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    unsigned long currentTime = millis();
    if (currentTime - lastClickTime < 700) {
      if (menuLevel == SUB_MENU_SERVOMOTOR) {
        encoderPos = 2;
        isChoosingAngle = false;
        isApplyingAngle = false;
      }
    }
    else {
      if (menuLevel == MAIN_MENU) {
        mainMenuCount = encoderPos;
        if (mainMenuCount == 1) {
          encoderPos = 128;//128
          menuLevel = SUB_MENU_GAIN;
      }
        else if (mainMenuCount == 2) {
          menuLevel = SUB_MENU_SERVOMOTOR;
          encoderPos = 1;
      }
    }
      else if (menuLevel == SUB_MENU_GAIN) {
        menuLevel = MAIN_MENU;
       encoderPos = 1;
    }
      else {
        servoMenuCount = encoderPos;
        if (servoMenuCount == 1 ) {
          if (isChoosingAngle == false) {
            isChoosingAngle = true;
        }
          else {
            isApplyingAngle = true;
            applyAngle(angle);
        }
        
      }
        else if (servoMenuCount == 2) {
          menuLevel = MAIN_MENU;
          encoderPos = 2;
      }
    
    }
    }
    lastClickTime = currentTime;
    menuChanged = true;
  } 
  lastButtonState = currentButtonState;

}
