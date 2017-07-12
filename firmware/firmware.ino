// This program is partly based on SSD1306ClockDemo example from the
// 'ESP8266_Oled_Driver_for_SSD1306_display' library.

#include <TimeLib.h>

#include <Wire.h>
#include "SSD1306.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Initialize the OLED display using Wire library
const int PIN_SDA = 5; // IO 17
const int PIN_SCL = 4; // IO 18
SSD1306 display(0x3c, PIN_SDA, PIN_SCL);

OLEDDisplayUi ui(&display);

// Screen dimensions.
static const int SCREEN_W = 64;
static const int SCREEN_H = 48;

int clockCenterX = SCREEN_W;
int clockCenterY = ((SCREEN_H - 16) / 2) + 6;

const int ECHO_PIN = 12;
const int TRIG_PIN = 13;

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

  if (second() % 2) {
    cm = getDistance();
  }

  String centimeters;
  if (cm > 80) {
    centimeters = "80+ cm";
  } else {
    centimeters = twoDigits(cm) + "  cm";
  }

  display->setFont(ArialMT_Plain_16);
  display->drawString(clockCenterX + x, clockCenterY + y + 20,
                      centimeters);
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
  // digitalWrite(23, LOW);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  attachInterrupt(0, handleButton, FALLING);

  initializeUI();

  display.flipScreenVertically();

  setTime(calculateEpoch());

}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  return microsecondsToCentimeters(duration);
}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {



    delay(remainingTimeBudget);
  }
}
