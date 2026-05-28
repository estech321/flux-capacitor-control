// Put in Boards Manager URLs:
// https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
//
// Must be uploaded from a Windows machine with the Adafruit drivers installed,
//     see lab notebook on 5/25/2026 for more info.
//
// Runs on Adafruit Trinket ATtiny85
// Eric Smith (mostly generated with ChatGPT and some Gemini)
// 2026-05-25
//
// Button cycles through modes:
// 0 = Steady Full On
// 1 = Slow Breathing
// 2 = Rapid Breathing
// 3 = Slow Blinking
// 4 = Rapid Blinking
// 5 = Fully Off
//
// CIRCUIT WIRING:
// Pin 0 > 2.2K Ohm Resistor > Base of 2N2222A NPN transistor > COLLECTOR to christmas lights/flux capacitor > 220 Ohm resistor > +5V, EMITTER to ground. Pin 2 > Pushbutton > Ground.

const int ledPin = 0;      // PWM output to 2N2222A transistor base
const int buttonPin = 2;   // Pushbutton to GND

int mode = 0;

// Debounce variables
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);

  // Uses internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  checkButton();

  switch (mode) {

    // -------------------------
    // Steady Full On
    // -------------------------
    case 0:
      analogWrite(ledPin, 255);
      delay(10);
      break;

    // -------------------------
    // Slow Breathing
    // -------------------------
    case 1:
      breathe(5);
      break;

    // -------------------------
    // Rapid Breathing
    // -------------------------
    case 2:
      breathe(1);
      break;

    // -------------------------
    // Slow Blinking
    // -------------------------
    case 3:
      blinkMode(500);
      break;

    // -------------------------
    // Rapid Blinking
    // -------------------------
    case 4:
      blinkMode(100);
      break;

    // -------------------------
    // Fully Off
    // -------------------------
    case 5:
      analogWrite(ledPin, 0);
      delay(10);
      break;
  }
}

// =====================================================
// Check button press and cycle modes
// =====================================================
void checkButton() {

  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != currentButtonState) {
      currentButtonState = reading;

      // Button pressed (active LOW)
      if (currentButtonState == LOW) {
        mode++;

        if (mode > 5) {
          mode = 0;
        }
      }
    }
  }

  lastButtonState = reading;
}

// =====================================================
// Breathing effect
// speedDelay controls breathing speed
// =====================================================
void breathe(int speedDelay) {

  // Fade in
  for (int i = 0; i <= 255; i++) {
    analogWrite(ledPin, i);
    delay(speedDelay);

    checkButton();
    if (mode != 1 && mode != 2) return;
  }

  // Fade out
  for (int i = 255; i >= 0; i--) {
    analogWrite(ledPin, i);
    delay(speedDelay);

    checkButton();
    if (mode != 1 && mode != 2) return;
  }
}

// =====================================================
// Blink effect
// interval = ON/OFF duration in ms
// =====================================================
void blinkMode(int interval) {

  analogWrite(ledPin, 255);
  delayWithButtonCheck(interval);

  analogWrite(ledPin, 0);
  delayWithButtonCheck(interval);
}

// =====================================================
// Delay while still checking button presses
// =====================================================
void delayWithButtonCheck(int duration) {

  unsigned long start = millis();

  while (millis() - start < duration) {
    checkButton();
    delay(1);
  }
}
