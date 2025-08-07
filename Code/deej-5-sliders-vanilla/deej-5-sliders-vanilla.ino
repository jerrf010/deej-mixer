#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// MCP3008 SPI Bit-Bang
const int CLK = 11;
const int DOUT = 13;
const int DIN = 12;
const int CS = 10;

#define NUM_STRIPS 5
#define LEDS_PER_STRIP 5

const int NUM_SLIDERS = 9;
const int NUM_ANALOG_SLIDERS = 8;
const int analogInputs[NUM_ANALOG_SLIDERS] = {A0, A1, A2, A3, A4, A5, A6, A7};

int analogSliderValues[NUM_SLIDERS];
int ledPins[NUM_STRIPS] = {2, 3, 4, 5, 6};
int mode = 0;
const int numModes = 4;

const int buttonPin = 8;
const int ledIndicatorPin = 7;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
unsigned long lastActivityTime = 0;
const unsigned long sleepTimeout = 20000; // 20 seconds
bool isSleeping = false;

Adafruit_NeoPixel* strips[NUM_STRIPS];

// MCP3008 reading function
int readMCP3008(int channel) {
  int adcValue = 0;
  digitalWrite(CS, LOW);
  byte command = 0b00011000 | (channel & 0x07);
  for (int i = 4; i >= 0; i--) {
    digitalWrite(DIN, (command >> i) & 0x01);
    digitalWrite(CLK, HIGH); delayMicroseconds(1);
    digitalWrite(CLK, LOW); delayMicroseconds(1);
  }
  for (int i = 0; i < 12; i++) {
    digitalWrite(CLK, HIGH); delayMicroseconds(1);
    digitalWrite(CLK, LOW); delayMicroseconds(1);
    adcValue <<= 1;
    if (digitalRead(DOUT)) adcValue |= 0x01;
  }
  digitalWrite(CS, HIGH);
  return (adcValue >> 1) & 0x3FF;
}

void setup() {
  // MCP3008 Pins
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(DOUT, INPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(CLK, LOW);

  // LED strips
  for (int i = 0; i < NUM_STRIPS; i++) {
    strips[i] = new Adafruit_NeoPixel(LEDS_PER_STRIP, ledPins[i], NEO_GRB + NEO_KHZ800);
    strips[i]->begin();
    strips[i]->show();
  }
  // Analog pins
  for (int i = 0; i < NUM_ANALOG_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  // Button and indicator
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledIndicatorPin, OUTPUT);

  Serial.begin(9600);

  // EEPROM Mode
  mode = EEPROM.read(0);
  if (mode >= numModes) mode = 0;

  // Startup animation: ripple
  for (int frame = 0; frame < LEDS_PER_STRIP + 2; frame++) {
    for (int stripIndex = 0; stripIndex < NUM_STRIPS; stripIndex++) {
      for (int led = 0; led < LEDS_PER_STRIP; led++) {
        if (led == frame - 1 || led == frame - 2) {
          strips[stripIndex]->setPixelColor(led, strips[stripIndex]->Color(255, 255, 255));
        } else {
          strips[stripIndex]->setPixelColor(led, 0);
        }
      }
      strips[stripIndex]->show();
    }
    delay(100);
  }
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < LEDS_PER_STRIP; j++) strips[i]->setPixelColor(j, 0);
    strips[i]->show();
  }
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // --- Detect button press ---
  if (buttonState == LOW && lastButtonState == HIGH) {
    lastActivityTime = millis();  // Activity detected

    if (isSleeping) {
      wakeFromSleep();
    } else {
      mode = (mode + 1) % numModes;
      EEPROM.write(0, mode);
      Serial.print("Mode changed to: ");
      Serial.println(mode);
    }
    delay(200); // Debounce
  }

  lastButtonState = buttonState;

  // --- Always check sliders and print serial ---
  updateSliderValues();
  sendSliderValues();

  // --- Enter sleep after inactivity ---
  if (!isSleeping && millis() - lastActivityTime > sleepTimeout) {
    enterSleepMode();
  }

  // --- If sleeping, skip animations but keep serial alive ---
  if (isSleeping) {
    delay(50);
    return;
  }

  // --- Indicator light shows active mode ---
  digitalWrite(ledIndicatorPin, (mode == 0) ? LOW : HIGH);

  // --- LED Modes ---
  if (mode == 0) {
    // MODE 0: All LEDs off
    for (int i = 0; i < NUM_STRIPS; i++) {
      for (int j = 0; j < LEDS_PER_STRIP; j++) {
        strips[i]->setPixelColor(j, 0);
      }
      strips[i]->show();
    }
  } else if (mode == 1) {
    // MODE 1: Slider Effects
    for (int stripIndex = 0; stripIndex < NUM_STRIPS; stripIndex++) {
      if (stripIndex == 4) {
        int valA0 = analogSliderValues[0];
        int valA1 = analogSliderValues[1];
        int valA2 = analogSliderValues[2];

        strips[stripIndex]->setPixelColor(1, strips[stripIndex]->Color(map(valA0, 0, 1023, 0, 255), 255 - map(valA0, 0, 1023, 0, 255), 0));
        strips[stripIndex]->setPixelColor(2, strips[stripIndex]->Color(map(valA1, 0, 1023, 0, 255), 255 - map(valA1, 0, 1023, 0, 255), 0));
        strips[stripIndex]->setPixelColor(3, strips[stripIndex]->Color(map(valA1, 0, 1023, 0, 255), 255 - map(valA1, 0, 1023, 0, 255), 0));
        strips[stripIndex]->setPixelColor(4, strips[stripIndex]->Color(map(valA2, 0, 1023, 0, 255), 255 - map(valA2, 0, 1023, 0, 255), 0));
      } else {
        int analogIndex = 6 - stripIndex;
        int val = analogSliderValues[analogIndex];
        int count = map(val, 0, 1023, 0, LEDS_PER_STRIP);

        uint32_t color;
        switch (stripIndex) {
          case 0: color = strips[stripIndex]->Color(255, 0, 0); break;
          case 1: color = strips[stripIndex]->Color(255, 128, 0); break;
          case 2: color = strips[stripIndex]->Color(0, 255, 0); break;
          case 3: color = strips[stripIndex]->Color(0, 0, 255); break;
        }

        for (int j = 0; j < LEDS_PER_STRIP; j++) {
          strips[stripIndex]->setPixelColor(j, (j < count) ? color : 0);
        }
      }
      strips[stripIndex]->show();
    }
  } else if (mode == 2) {
    // MODE 2: Rainbow Animation
    static uint16_t rainbowOffset = 0;
    for (int i = 0; i < NUM_STRIPS; i++) {
      for (int j = 0; j < LEDS_PER_STRIP; j++) {
        uint16_t hue = (j * 65536L / LEDS_PER_STRIP + rainbowOffset) % 65536;
        strips[i]->setPixelColor(j, strips[i]->ColorHSV(hue));
      }
      strips[i]->show();
    }
    rainbowOffset += 256;
  } else if (mode == 3) {
    // MODE 3: RGB Ripple Effect
    static int rippleFrame = 0;
    static uint16_t hueBase = 0;

    for (int stripIndex = 0; stripIndex < NUM_STRIPS; stripIndex++) {
      for (int led = 0; led < LEDS_PER_STRIP; led++) {
        uint32_t color = 0;
        if (led == rippleFrame) {
          color = strips[stripIndex]->ColorHSV(hueBase, 255, 255);
        } else if (led == rippleFrame - 1 || led == rippleFrame + 1) {
          color = strips[stripIndex]->ColorHSV(hueBase, 255, 100);
        }
        strips[stripIndex]->setPixelColor(led, color);
      }
      strips[stripIndex]->show();
    }

    rippleFrame = (rippleFrame + 1) % LEDS_PER_STRIP;
    hueBase += 700;
    delay(150);
    return;
  }

  delay(10); // slight delay for stability
}

void updateSliderValues() {
  static int prevSliderValues[NUM_SLIDERS];
  bool sliderMoved = false;

  for (int i = 0; i < NUM_ANALOG_SLIDERS; i++) {
    int val = analogRead(analogInputs[i]);
    analogSliderValues[i] = val;

    if (abs(val - prevSliderValues[i]) > 30) {
      sliderMoved = true;
    }
    prevSliderValues[i] = val;
  }

  // MCP3008 as 9th "slider"
  int mcpVal = readMCP3008(0);
  analogSliderValues[8] = mcpVal;
  if (abs(mcpVal - prevSliderValues[8]) > 20) sliderMoved = true;
  prevSliderValues[8] = mcpVal;

  if (sliderMoved) {
    lastActivityTime = millis();
    if (isSleeping) wakeFromSleep();
  }
}

void sendSliderValues() {
  String builtString = "";

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String(analogSliderValues[i]);
    if (i < NUM_SLIDERS - 1) {
      builtString += "|";
    }
  }
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void enterSleepMode() {
  isSleeping = true;
  Serial.println("Entering sleep mode...");
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < LEDS_PER_STRIP; j++) {
      strips[i]->setPixelColor(j, 0);
    }
    strips[i]->show();
  }
  digitalWrite(ledIndicatorPin, LOW);
}

void wakeFromSleep() {
  isSleeping = false;
  Serial.println("Waking from sleep...");
}