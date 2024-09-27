#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create SSD1306 display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define the Servo object
Servo myServo;

// Pin assignments
const int servoPin = 9; // Pin connected to the servo motor
const int potPin = A0;  // Pin connected to the potentiometer
const int joyXPin = A1; // Pin connected to the joystick X-axis

int servoPos = 90;     // Initial position for the servo (center)
int potValue = 0;      // To store potentiometer value
int joyXValue = 0;     // To store joystick X-axis value
int lastServoPos = 90; // To store the last servo position to avoid unnecessary updates

// Dead zone range
int calibratedCenter = 512; // Will be dynamically calibrated
int deadZoneMin;            // Minimum joystick value for "center" (calculated dynamically)
int deadZoneMax;            // Maximum joystick value for "center" (calculated dynamically)

// Function to smooth analog readings
int readSmooth(int pin)
{
  int total = 0;
  for (int i = 0; i < 10; i++)
  { // Average 10 readings
    total += analogRead(pin);
    delay(5); // Small delay to reduce noise between readings
  }
  return total / 10; // Return the average value
}

void setup()
{
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Initialize the OLED display with I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);              // Set the font size
  display.setTextColor(SSD1306_WHITE); // Set the color to white
  display.setCursor(0, 0);
  display.print("Calibrating...");
  display.display(); // Show "Calibrating..." message on the OLED

  // **Dynamic Calibration**: Capture the joystick center value at startup
  delay(2000);                            // Allow time to ensure the joystick is at rest
  calibratedCenter = readSmooth(joyXPin); // Capture initial joystick position as center

  // Define dynamic dead zone around the calibrated center
  deadZoneMin = calibratedCenter - 20; // 20 units below the center
  deadZoneMax = calibratedCenter + 20; // 20 units above the center

  // Display calibrated center and dead zone values on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Center Calibrated: ");
  display.print(calibratedCenter);
  display.display();
  delay(2000); // Display calibration result for 2 seconds

  // Attach the servo motor
  myServo.attach(servoPin);
  myServo.write(servoPos); // Set servo to the center position initially
}

void loop()
{
  // Smooth joystick X-axis value
  joyXValue = readSmooth(joyXPin); // Using the readSmooth function

  // Print joystick value and calibrated center to Serial Monitor for debugging
  Serial.print("Joystick X Value: ");
  Serial.println(joyXValue);
  Serial.print("Calibrated Center: ");
  Serial.println(calibratedCenter);

  // Display joystick value on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Joystick X Value: ");
  display.print(joyXValue); // Show the real-time joystick X-axis value

  display.setCursor(0, 10);
  display.print("Center: ");
  display.print(calibratedCenter); // Display the calibrated center value

  // Display the dead zone range for clarity
  display.setCursor(0, 20);
  display.print("Dead Zone: ");
  display.print(deadZoneMin);
  display.print(" - ");
  display.print(deadZoneMax);

  // Update the OLED with these values
  display.display();

  // Map joystick input to servo angle, but with the dynamic dead zone around the calibrated center
  if (joyXValue < deadZoneMin)
  {
    // If the joystick is pushed to the left
    servoPos = map(joyXValue, 0, deadZoneMin, 0, 85); // Move from 0 to 85 degrees
  }
  else if (joyXValue > deadZoneMax)
  {
    // If the joystick is pushed to the right
    servoPos = map(joyXValue, deadZoneMax, 1023, 95, 180); // Move from 95 to 180 degrees
  }
  else
  {
    // Joystick is in the center dead zone
    servoPos = 90; // Keep servo at center
  }

  // Only update servo if the position has significantly changed
  if (abs(servoPos - lastServoPos) > 2)
  { // Only move if change is > 2 degrees
    myServo.write(servoPos);
    lastServoPos = servoPos;
  }

  // Smooth potentiometer value for speed control
  potValue = readSmooth(potPin);
  int delaySpeed = map(potValue, 0, 1023, 0, 50); // Adjust delay for speed

  // Add a delay based on potentiometer for speed control
  delay(delaySpeed);
}
