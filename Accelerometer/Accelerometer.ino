
/*
  ADXL3xx

  Reads an Analog Devices ADXL3xx accelerometer and communicates the
  acceleration to the computer.  The pins used are designed to be easily
  compatible with the breakout boards from Sparkfun, available from:
  http://www.sparkfun.com/commerce/categories.php?c=80

  http://www.arduino.cc/en/Tutorial/ADXL3xx

  The circuit:
  analog 0: accelerometer self test
  analog 1: z-axis
  analog 2: y-axis
  analog 3: x-axis
  analog 4: ground
  analog 5: vcc

  created 2 Jul 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

*/

// these constants describe the pins. They won't change:
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int pinX = A3;                  // x-axis of the accelerometer
const int pinY = A2;                  // y-axis
const int pinZ = A1;                  // z-axis (only on 3-axis models)

const int pinWhite = 13;
const int pinBlue = 12;

// Settings
int maxValue = 550;
int minValue = 500;

// Values
int xAcceleration = 0;
int yAcceleration = 0;
int zAcceleration = 0;
int loopCount = 0;

void setup() {
  Serial.begin(9600); // Start communication

  // Provide ground and power by using the analog inputs as normal
  // digital pins.  This makes it possible to directly connect the
  // breakout board to the Arduino.  If you use the normal 5V and
  // GND pins on the Arduino, you can remove these lines.
  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

  pinMode(pinWhite, OUTPUT);
  pinMode(pinBlue, OUTPUT);

  // Start log
  Serial.println("X\tY\tZ\t");
}

void loop() {
	xAcceleration = analogRead(pinX);
	yAcceleration = analogRead(pinY);
	zAcceleration = analogRead(pinZ);
	loopCount++;

	Serial.print("Loop "); Serial.print(loopCount);
	Serial.print("\tX:\t"); Serial.print(xAcceleration);
	Serial.print("\tY:\t"); Serial.print(yAcceleration);
	Serial.print("\tZ:\t"); Serial.println(zAcceleration);

	delay(3000);

	if (xAcceleration < minValue ||
		xAcceleration > maxValue ||
		yAcceleration < minValue ||
		yAcceleration > maxValue ||
		zAcceleration < minValue ||
		zAcceleration > maxValue) {

		if (loopCount < 100) {
			digitalWrite(pinWhite, HIGH);
			digitalWrite(pinBlue, LOW);
		} else if (loopCount < 130) {
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
