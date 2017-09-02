// This program is partly based on SSD1306ClockDemo example from the
// 'ESP8266_Oled_Driver_for_SSD1306_display' library.

#include <TimeLib.h>

#include <Wire.h>
#include "SSD1306.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Initialize the OLED display using Wire library
const int PIN_SDA = 18; // IO 17
const int PIN_SCL = 17; // IO 18
SSD1306 display(0x3c, PIN_SDA, PIN_SCL);

OLEDDisplayUi ui(&display);

// Screen dimensions.
static const int SCREEN_W = 64;
static const int SCREEN_H = 48;

int clockCenterX = SCREEN_W;
int clockCenterY = ((SCREEN_H - 16) / 2) + 6;

/* Led */
static const int LED_PIN_1 = 13;
static const int LED_PIN_2 = 14;

// utility function for digital clock display: prints leading 0
static String twoDigits(int digits) {
  if (digits < 10) {
    return String('0' + String(digits));
  } else {
    return String(digits);
  }
}

volatile long cm = 0;

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String timenow;
  if (second() % 2) {
    timenow = String(hour()) + " " + twoDigits(minute());
  } else {
    timenow = String(hour()) + ":" + twoDigits(minute());
  }

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x, clockCenterY + y, timenow);

 // if (second() % 2) {
//    cm = getDistance();
 // }

//  String centimeters;
//  if (cm > 80) {
//    centimeters = "80+ cm";
//  } else {
//    centimeters = twoDigits(cm) + "  cm";
//  }

//
}

FrameCallback frames[] = { digitalClockFrame };
int frameCount = 1;

// Calculate Unix time epoch; return the epoch in seconds.
static unsigned long calculateEpoch() {
  unsigned long secsSinceStart = millis();
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long SEVENTY_YEARS = 2208988800UL;
  return secsSinceStart - (SEVENTY_YEARS * SECS_PER_HOUR);
}

void initializeUI() {
  ui.setTargetFPS(60);
  ui.setIndicatorPosition(TOP);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.disableAutoTransition();

  // Initialising the UI will init the display too.
  ui.init();
}

void handleButton() {
  // Serial.println("hi");
  //   digitalWrite(23, ! digitalRead(23));
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  // pinMode(23, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, LOW);

  attachInterrupt(0, handleButton, FALLING);

  initializeUI();

  display.flipScreenVertically();

  setTime(calculateEpoch());

}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {



    delay(remainingTimeBudget);
  }
}
