#include "EasyButton.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "RTClib.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define BUTTON_1_PIN 5
#define BUTTON_2_PIN 6
#define BUTTON_3_PIN 9
#define BUTTON_4_PIN 8

#define DHTTYPE DHT11
#define DHTPIN 3

int debounce = 50;
int menuInput = 0;
int menuCount = 1;
char *menu[] { "Weather", "Time" };
char *dateTimeSteps[] { "Day", "Month", "Year", "Hour", "Minute", "Second" };
int inputDateTime[6] = {};
int currentStep = 0;
bool isUpdate = false;
DateTime defaultDateTime;
DateTime now;
bool isAdjust = false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
EasyButton button_1(BUTTON_1_PIN, debounce);
EasyButton button_2(BUTTON_2_PIN, debounce);
EasyButton button_3(BUTTON_3_PIN, debounce);
EasyButton button_4(BUTTON_4_PIN, debounce);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  rtc.begin();

  button_1.begin();
  button_1.enableInterrupt(buttonISR_1);
  button_1.onPressed(changeMenu);

  button_2.begin();
  button_2.enableInterrupt(buttonISR_2);
  button_2.onPressed(selectEvent);

  button_3.begin();
  button_3.enableInterrupt(buttonISR_3);
  button_3.onPressed(subDate);

  button_4.begin();
  button_4.enableInterrupt(buttonISR_4);
  button_4.onPressed(addDate);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
	{ // Address for 128x64
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}
}

void loop() {
  delay(50);
  display.clearDisplay();
  display.setTextColor(WHITE); // Draw white text
	display.setCursor(0,0);
  now = rtc.now();
  if(isUpdate){
    printDateTime(now);
    display.println();
    display.println();
    printStep();
  }
  else if(isAdjust){
      rtc.adjust(DateTime(inputDateTime[2], inputDateTime[1], inputDateTime[0], inputDateTime[3], inputDateTime[4], inputDateTime[5]));
      isAdjust = false;
  }
  else{
    switch (menuInput) {
      case 1:
        printWeather();
        break;
      case 2:
        printDateTime(now);
        break;
      default:
        printMenu();
        break;
    }
  }
  display.display();
}

// Function that displays a menu
void printMenu(){
  for (int i = 0; i < sizeof(menu)/4; i++) {
    if (i + 1 == menuCount) {
      display.fillRoundRect(0, display.getCursorY(), SCREEN_WIDTH,8, 0, WHITE);
      display.setTextColor(BLACK);
    }
    else {
      display.setTextColor(WHITE);
    }
    display.print(i+1);
    display.print('.');
    display.println(menu[i]);
  }
}

// Function that draws a degree symbol on display
void drawDegree(){
  display.drawCircle(display.getCursorX()-4, display.getCursorY()+2, 2, WHITE);
}

// Function that gets and displays temperature and humidity
void printWeather(){
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  display.print(F("Humidity: "));
  display.print(humidity);
  display.println('%');
  display.print(F("Temperature: "));
  display.print(temperature);
  display.print("  ");
  drawDegree();
  display.println(F("C"));
}

// Function that adds "0" to a string if it contains 1 digit
String insertChar(int val){
  if(val < 10){
    return String('0') + String(val);
  }
  return String(val);
}

// Function that displays current date and time
void printDateTime(DateTime now){
  display.print(insertChar(now.day()));
  display.print('/');
  display.print(insertChar(now.month()));
  display.print('/');
  display.println(insertChar(now.year()));

  display.print(insertChar(now.hour()));
  display.print(':');
  display.print(insertChar(now.minute()));
  display.print(':');
  display.print(insertChar(now.second()));
}

// Function that displays current step date value
void printStep(){
  display.println(dateTimeSteps[currentStep]);
  switch (currentStep) {
    case 0:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
    case 1:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
    case 2:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
    case 3:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
    case 4:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
    case 5:
      display.print(insertChar(inputDateTime[currentStep]));
      break;
  }
}

// Function that returns the current step date value
int getDateTimeStep(DateTime datetime){
  switch (currentStep) {
    case 0:
      return datetime.day();
      break;
    case 1:
      return datetime.month();
      break;
    case 2:
      return datetime.year();
      break;
    case 3:
      return datetime.hour();
      break;
    case 4:
      return datetime.minute();
      break;
    case 5:
      return datetime.second();
      break;
  }
}


// Interrupt services
void buttonISR_1(){
  button_1.read();
}

void buttonISR_2(){
  button_2.read();
}

void buttonISR_3(){
  button_3.read();
}

void buttonISR_4(){
  button_4.read();
}

// Event that either select a menu option or adjust the time
void selectEvent(){
  if(menuInput == 0){
    menuInput = menuCount;
  }
  else if(menuInput == 2 && !isUpdate) {
    isUpdate = true;
    defaultDateTime = now;
    inputDateTime[0] = defaultDateTime.day();
  }
  else if (isUpdate) {
    if (currentStep < 5) {
      currentStep++;
      inputDateTime[currentStep] = getDateTimeStep(defaultDateTime);
    }
    else{
      isUpdate = false;
      isAdjust = true;
    }
  }
}

// Event that either marks the current menu option or goes back to menu
void changeMenu(){
  if (menuInput > 0) {
    menuInput = 0;
    isUpdate = false;
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

// Event that subtracts from the selected date step
void subDate(){
  if(inputDateTime[currentStep] > 1){
    inputDateTime[currentStep]--;
  }
}

// Event that adds to the selected date step
void addDate(){
  switch(currentStep){
    case 0:
      if(inputDateTime[currentStep] < 31){
        inputDateTime[currentStep]++;
      }
    break;
    case 1:
      if(inputDateTime[currentStep] < 12){
        inputDateTime[currentStep]++;
      }
    break;
    case 2:
      inputDateTime[currentStep]++;
    break;
    case 3:
      if(inputDateTime[currentStep] < 24){
        inputDateTime[currentStep]++;
      }
    break;
    case 4:
      if(inputDateTime[currentStep] < 60){
        inputDateTime[currentStep]++;
      }
    break;
    case 5:
      if(inputDateTime[currentStep] < 60){
        inputDateTime[currentStep]++;
      }
    break;
  }
}