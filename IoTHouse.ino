#include "EasyButton.h"
#define BUTTON_1_PIN 5
#define BUTTON_2_PIN 6
int debounce = 50;
int menuInput = 0;
int menuCount = 1;
bool selected = false;

EasyButton button_1(BUTTON_1_PIN, debounce);
EasyButton button_2(BUTTON_2_PIN, debounce);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  button_1.begin();
  button_1.enableInterrupt(buttonISR_1);
  button_1.onPressed(changeMenu);

  button_2.begin();
  button_2.enableInterrupt(buttonISR_2);
  button_2.onPressed(selectEvent);
}

void loop() {
  switch (menuInput) {
   case 1:
    time();
    break;
   case 2:
    temperature();
    break;
   default:
    printMenu();
    break;
  }
  delay(2000);
}


void printMenu(){
  Serial.println("1. Time");
  Serial.println("2. Temperature");
  Serial.println(menuCount);
}

void temperature(){
  Serial.print("temp = ");
  Serial.println(menuInput);
}

void time(){
  Serial.println("nothing here");
}

void buttonISR_1(){
  button_1.read();
}

void buttonISR_2(){
  button_2.read();
}

void selectEvent(){
  menuInput = menuCount;
}

void changeMenu(){
  if (menuInput > 0) {
    menuInput = 0;
  }
  else {  
    if (menuCount == 2) {
      menuCount = 1;
    }
    else {
      menuCount++;
    }
  }
}