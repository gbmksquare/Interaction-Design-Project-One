
/*
  ADXL3xx
  http://www.arduino.cc/en/Tutorial/ADXL3xx
*/

// these constants describe the pins. They won't change:
const int pinGround = 18; 
const int pinPower = 19;  
const int pinX = A3;      
const int pinY = A2;      
const int pinZ = A1;      

int pinsWhite[3] = {9, 10, 11};
int pinBlue = 6;

const int COUNT_PINS_WHITE = 3;

int lightOnWhite = false;
int lightOnBlue = false;

// Settings
int calibrationX = 0;
int calibrationY = 0;
int calibrationZ = 0;
int calibrationOffset = 50;
int fadeDelay = 1;

// Values
int accelerationX = 0;
int accelerationY = 0;
int accelerationZ = 0;
int loopCount = 0;

// Log
void logHeader() {
  Serial.println("X\tY\tZ\t");
}

void logAcceleration() {
  Serial.print("Loop "); Serial.print(loopCount);
  Serial.print("\tX:\t"); Serial.print(accelerationX);
  Serial.print("\tY:\t"); Serial.print(accelerationY);
  Serial.print("\tZ:\t"); Serial.println(accelerationZ);
}

// Setup
void setOutputPin(int pin) {
  pinMode(pin, OUTPUT);
}

void setCalibration() {
  calibrationX = accelerationX;
  calibrationY = accelerationY;
  calibrationZ = accelerationZ;
  Serial.println("Set calibration:");
  logAcceleration();
}

void readAcceleration() {
  accelerationX = analogRead(pinX);
  accelerationY = analogRead(pinY);
  accelerationZ = analogRead(pinZ);
}

// Light
void lightsFadeIn(int pins[], int pinsCount, int start, int end) {
  if (start < 0) { start = 0; }
  if (end > 255) { end = 255; }
  if (start > end) { return; }
  for (int i = start; i <= end; i++) {
    for (int index = 0; index < pinsCount; index++) {
      int pin = pins[index];
      analogWrite(pin, i);
      delay(fadeDelay);
    }
  }
}

void lightFadeIn(int pin, int start, int end) {
  int pins[] = {pin};
  lightsFadeIn(pins, 1, start, end);
}

void lightFadeIn(int pin) {
  lightFadeIn(pin, 0, 255);
}

void lightsFadeOut(int pins[], int pinsCount, int start, int end) {
  if (start < 0) { start = 0; }
  if (start > 255) { end = 255; }
  if (start < end) { return; }
  for (int i = start; i >= end; i--) {
    for (int index = 0; index < pinsCount; index++) {
      int pin = pins[index];
      analogWrite(pin, i);
      delay(fadeDelay);
    }
  }
}

void lightFadeOut(int pin, int start, int end) {
  int pins[] = {pin};
  lightsFadeOut(pins, 1, start, end);
}

void lightFadeOut(int pin) {
  lightFadeOut(pin, 255, 0);
}

void lightsFlicker(int pins[], int pinsCount, int flickerCount) {
  for (int i = 0; i < flickerCount; i++) {
    lightsFadeIn(pins, pinsCount, 0, 255);
    lightsFadeOut(pins, pinsCount, 255, 0);
  }
}

void lightFlicker(int pin, int flickerCount) {
  int pins[] = {pin};
  lightsFlicker(pins, 1, flickerCount);
}

// Helper
bool shouldLightOn() {
  if (accelerationX < calibrationX - calibrationOffset ||
    accelerationX > calibrationX + calibrationOffset ||
    accelerationY < calibrationY - calibrationOffset ||
    accelerationY > calibrationY + calibrationOffset ||
    accelerationZ < calibrationZ - calibrationOffset ||
    accelerationZ > calibrationZ + calibrationOffset) {
      return true;
    } else {
      return false;
    }
}

// Application
void setup() {
  // Setup
  Serial.begin(9600); // Start communication

  setOutputPin(pinGround);
  setOutputPin(pinPower);
  setOutputPin(pinBlue);
  for (int i = 0; i < COUNT_PINS_WHITE; i++) {
    int pin = pinsWhite[i];
    setOutputPin(pin);
  }

  digitalWrite(pinGround, LOW);
  digitalWrite(pinPower, HIGH);

  // Calibration
  readAcceleration();
  setCalibration();

  // Ready
  logHeader();
  lightsFlicker(pinsWhite, COUNT_PINS_WHITE, 2);
  lightFadeIn(pinBlue);
  delay(1000);
}

void loop() {
  readAcceleration();
  logAcceleration();

  bool shouldActivate = shouldLightOn();

  if (shouldActivate == true) {
    // Turn on white, turn off blue
    if (lightOnWhite == false) {
      lightsFadeIn(pinsWhite, COUNT_PINS_WHITE, 0, 255);
      lightOnWhite = true;
    }
    if (lightOnBlue == true) {
      lightFadeOut(pinBlue);
      lightOnBlue = false;
    }
  } else {
    // Turn off white, turn on blue
    if (lightOnWhite == true) {
      lightsFadeOut(pinsWhite, COUNT_PINS_WHITE, 255, 0);
      lightOnWhite = false;
    }
    if (lightOnBlue == false) {
      lightFadeIn(pinBlue, 0, 255);
      lightOnBlue = true;
    }
  }
  loopCount++;
}
