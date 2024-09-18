#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int trigPin = 2;       // ultrasonic trig pin
const int echoPin = 3;       // ultrasonic echo pin
const int servo1Pin = 9;     // servo 1 signal pin
const int servo2Pin = 10;    // servo 2 signal pin
const int oledResetPin = -1; // oled reset pin (not used in this case)
const float maxDist = 400;   // Maximum distance the sensor can measure (in cm)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oledResetPin);
Servo servo1, servo2;

int servo1Angle = 90; // Initial angle for servo 1
int servo2Angle = 90; // Initial angle for servo 2

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(servo1Angle);
  servo2.write(servo2Angle);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize oled
  display.display();
  delay(2000); // pause for 2 seconds
}

float getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void displayInfo(float distance, int servo1Angle, int servo2Angle)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");
  display.print("Servo1 Angle: ");
  display.println(servo1Angle);
  display.print("Servo2 Angle: ");
  display.println(servo2Angle);
  if (distance < 30)
  {
    display.println("Object Detected!");
  }
  else if (distance > maxDist)
  {
    display.println("Out of Range!");
  }
  display.display();
}

void moveServo(float distance)
{
  if (distance < 30)
  {
    if (distance < 10)
    {
      servo1Angle = 90;
      servo2Angle = 90;
    }
    else if (distance < 20)
    {
      servo1Angle = 0;
      servo2Angle = 0;
    }
    else if (distance < 30)
    {
      servo1Angle = 180;
      servo2Angle = 180;
    }
    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
  }
  else if (distance > maxDist)
  {
    // Handle out-of-range distance
    servo1.write(90); // Reset to default position
    servo2.write(90); // Reset to default position
  }
}

void loop()
{
  float distance = getDistance();
  if (distance <= maxDist)
  {
    moveServo(distance);
  }
  else
  {
    // Handle out-of-range distance
    servo1.write(90); // Reset to default position
    servo2.write(90); // Reset to default position
  }
  displayInfo(distance, servo1Angle, servo2Angle);
  delay(100); // Small delay to avoid excessive updates
}
