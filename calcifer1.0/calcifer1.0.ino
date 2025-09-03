#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

#define BUTTON_BACK    2
#define BUTTON_FORWARD 3
#define BUTTON_HOME    4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* texts[] = {
  "Text 1: Hello!",
  "Text 2: Arduino",
  "Text 3: OLED Rocks!"
};
const int numTexts = sizeof(texts) / sizeof(texts[0]);

int currentIndex = 0;
bool homeScreen = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_BACK, INPUT);
  pinMode(BUTTON_FORWARD, INPUT);
  pinMode(BUTTON_HOME, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  displayText(currentIndex);
}

void loop() {
  if (digitalRead(BUTTON_HOME) == HIGH) {
    homeScreen = true;
    displayHome();
    delay(300); // debounce
  }
  else if (!homeScreen) {  // Only navigate if not on home screen
    if (digitalRead(BUTTON_FORWARD) == HIGH) {
      currentIndex++;
      if (currentIndex >= numTexts) currentIndex = numTexts - 1;
      displayText(currentIndex);
      delay(300);
    }
    if (digitalRead(BUTTON_BACK) == HIGH) {
      currentIndex--;
      if (currentIndex < 0) currentIndex = 0;
      displayText(currentIndex);
      delay(300);
    }
  }
  else {  // On home screen, wait for back or forward to exit home
    if (digitalRead(BUTTON_FORWARD) == HIGH || digitalRead(BUTTON_BACK) == HIGH) {
      homeScreen = false;
      displayText(currentIndex);
      delay(300);
    }
  }
}

void displayText(int index) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(texts[index]);
  display.display();
}

void displayHome() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  const char* text = "0w0";

  // Variables to store bounding box
  int16_t x1, y1;
  uint16_t w, h;

  // Get the width and height of the text
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Calculate centered position
  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;

  display.setCursor(x, y);
  display.println(text);
  display.display();
}
