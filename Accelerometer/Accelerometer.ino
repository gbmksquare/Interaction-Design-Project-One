
/*
  ADXL3xx
  http://www.arduino.cc/en/Tutorial/ADXL3xx
*/

// these constants describe the pins. They won't change:
const int pinGround = 18;             // analog input pin 4 -- ground
const int pinPower = 19;              // analog input pin 5 -- voltage
const int pinX = A3;                  // x-axis of the accelerometer
const int pinY = A2;                  // y-axis
const int pinZ = A1;                  // z-axis (only on 3-axis models)

const int pinWhite = 13;
const int pinBlue = 12;

// Settings
int calibrationX = 0;
int calibrationY = 0;
int calibrationZ = 0;
int offset = 50;

// Values
int accelerationX = 0;
int accelerationY = 0;
int accelerationZ = 0;
int loopCount = 0;

void logHeader() {
  Serial.println("X\tY\tZ\t");
}

void logAcceleration() {
  Serial.print("Loop "); Serial.print(loopCount);
  Serial.print("\tX:\t"); Serial.print(accelerationX);
  Serial.print("\tY:\t"); Serial.print(accelerationY);
  Serial.print("\tZ:\t"); Serial.println(accelerationZ);
}

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
  loopCount++;
}

void setup() {
  // Setup
  Serial.begin(9600); // Start communication

  setOutputPin(pinGround);
  setOutputPin(pinPower);
  setOutputPin(pinWhite);
  setOutputPin(pinBlue);

  digitalWrite(pinGround, LOW);
  digitalWrite(pinPower, HIGH);

  // Calibration
  readAcceleration();
  setCalibration();

  logHeader();

  // Ready
  // TODO: Flicker twice
  delay(5000);
}

void loop() {
  readAcceleration();
  logAcceleration();

  if (accelerationX < calibrationX - offset ||
    accelerationX > calibrationX + offset ||
    accelerationY < calibrationY - offset ||
    accelerationY > calibrationY + offset ||
    accelerationZ < calibrationZ - offset ||
    accelerationZ > calibrationZ + offset) {

    if (loopCount < 100) {
      Serial.println("BLUE");
      digitalWrite(pinWhite, HIGH);
      digitalWrite(pinBlue, LOW);
    } else if (loopCount < 130) {
      Serial.println("YELLOW");
      digitalWrite(pinWhite, LOW);
      digitalWrite(pinBlue, HIGH);
    } else {
      loopCount = 0;
    }
  } else {
    digitalWrite(pinWhite, LOW);
    digitalWrite(pinBlue, LOW);
  }
}
