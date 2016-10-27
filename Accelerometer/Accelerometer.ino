
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

// Settings
int calibrationX = 0;
int calibrationY = 0;
int calibrationZ = 0;
int calibrationOffset = 50;
int fadeDelay = 2;

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
void lightFadeIn(int pin, int start, int end) {
  if (start < 0) { start = 0; }
  if (end > 255) { end = 255; }
  if (start > end) { return; }
  for (int i = start; i <= end; i++) {
    analogWrite(pin, i);
  }
}

void lightFadeIn(int pin) {
  lightFadeIn(pin, 0, 255);
}

void lightFadeOut(int pin, int start, int end) {
  if (start < 0) { start = 0; }
  if (start > 255) { end = 255; }
  if (start < end) { return; }
  for (int i = start; i >= end; i--) {
    analogWrite(pin, i);
    delay(fadeDelay);
  }
}

void lightFadeOut(int pin) {
  lightFadeOut(pin, 255, 0);
}

void lightFlicker(int pin, int count) {
  for (int i = 0; i < count; i++) {
    lightFadeIn(pin);
    lightFadeOut(pin);
  }
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

int count(int array[]) {
  return sizeof(array) / sizeof(int);
}

// Application
void setup() {
  // Setup
  Serial.begin(9600); // Start communication

  setOutputPin(pinGround);
  setOutputPin(pinPower);
  setOutputPin(pinBlue);
  for (int i = 0; i < count(pinsWhite); i++) {
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
  lightFlicker(pinsWhite[0], 2);
  lightFadeIn(pinBlue, 255/2);
  delay(5000);
}

void loop() {
  readAcceleration();
  logAcceleration();
  
  if (shouldLightOn() == true) {
    // All lights on
    for (int i = 0; i < count(pinsWhite); i++) {
      lightFadeIn(pinsWhite[i]);
    }
    lightFadeIn(pinBlue);
  } else {
    // Blue light on, White lights off
    for (int i = 0; i < count(pinsWhite); i++) {
      lightFadeOut(pinsWhite[i]);
    }
    lightFadeIn(pinBlue, 0, 255/2);
  }
  loopCount++;
}
