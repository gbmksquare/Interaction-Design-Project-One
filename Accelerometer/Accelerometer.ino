
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

const int pinWhite = 10;
const int pinBlue = 9;

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

void lightFadeIn(int pin) {
  for (int i = 0; i <= 255; i++) {
    analogWrite(pin, i);
    delay(fadeDelay);
  }
}

void lightFadeOut(int pin) {
  for (int i = 255; i >=0; i--) {
    analogWrite(pin, i);
    delay(fadeDelay);
  }
}

void lightFlicker(int count) {
  for (int i = 0; i < count; i++) {
    lightFadeIn(pinWhite);
    lightFadeOut(pinWhite);
  }
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
  lightFlicker(2);
  delay(5000);
}

void loop() {
  readAcceleration();
  logAcceleration();

  if (accelerationX < calibrationX - calibrationOffset ||
    accelerationX > calibrationX + calibrationOffset ||
    accelerationY < calibrationY - calibrationOffset ||
    accelerationY > calibrationY + calibrationOffset ||
    accelerationZ < calibrationZ - calibrationOffset ||
    accelerationZ > calibrationZ + calibrationOffset) {

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
