#include "EasyButton.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define BUTTON_1_PIN 5
#define BUTTON_2_PIN 6

#define DHTTYPE DHT11
#define DHTPIN 3

int debounce = 50;
int menuInput = 0;
int menuCount = 1;
bool selected = false;
char *menu[] { "Weather", "Time" };

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
EasyButton button_1(BUTTON_1_PIN, debounce);
EasyButton button_2(BUTTON_2_PIN, debounce);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  button_1.begin();
  button_1.enableInterrupt(buttonISR_1);
  button_1.onPressed(changeMenu);

  button_2.begin();
  button_2.enableInterrupt(buttonISR_2);
  button_2.onPressed(selectEvent);

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
  switch (menuInput) {
   case 1:
    printWeather();
    break;
   case 2:
    time();
    break;
   default:
    printMenu();
    break;
  }
  display.display();
}


void printMenu(){
	display.setCursor(0,1);
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

void drawDegree(){
  display.drawCircle(display.getCursorX()-4, display.getCursorY()+2, 2, WHITE);
}

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

void time(){
  display.print("time = ");
  display.println(menuInput);
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