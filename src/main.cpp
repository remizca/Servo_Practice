#include <Arduino.h>  // Include the Arduino library
#include <Servo.h>  // Include the Servo library
#include <LiquidCrystal.h>  // Include the LiquidCrystal library

Servo myservo;  // Create a servo object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Create an LCD object

const int buttonPin1 = 7;  // Pin for the first button
const int buttonPin2 = 8;  // Pin for the second button
int angle = 90;  // Initial angle for the servo

void updateLCD();  // Declare the updateLCD function

void setup() {
  myservo.attach(9);  // Attaches the servo on pin 9 to the servo object
  myservo.write(angle);  // Initialize servo to 90 degrees
  pinMode(buttonPin1, INPUT_PULLUP);  // Set button pin 1 as input with internal pull-up resistor
  pinMode(buttonPin2, INPUT_PULLUP);  // Set button pin 2 as input with internal pull-up resistor
  
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Angle: ");  // Print initial text
  lcd.setCursor(0, 1);  // Move cursor to the second row
  lcd.print("Position: ");  // Print initial text
  updateLCD();  // Update the LCD with the initial angle and position
}

void loop() {
  if (digitalRead(buttonPin1) == LOW) {  // Check if button 1 is pressed
    angle += 1;  // Increase the angle
    if (angle > 180) angle = 180;  // Limit the angle to 180 degrees
    myservo.write(angle);  // Move the servo to the new angle
    updateLCD();  // Update the LCD with the new angle and position
    delay(15);  // Wait for the servo to reach the position
  }
  
  if (digitalRead(buttonPin2) == LOW) {  // Check if button 2 is pressed
    angle -= 1;  // Decrease the angle
    if (angle < 0) angle = 0;  // Limit the angle to 0 degrees
    myservo.write(angle);  // Move the servo to the new angle
    updateLCD();  // Update the LCD with the new angle and position
    delay(15);  // Wait for the servo to reach the position
  }
}

void updateLCD() {
  lcd.setCursor(7, 0);  // Move cursor to the position after "Angle: "
  lcd.print(angle);  // Print the current angle
  lcd.print("   ");  // Clear any leftover characters
  lcd.setCursor(10, 1);  // Move cursor to the position after "Position: "
  lcd.print(angle);  // Print the current position (same as angle in this case)
  lcd.print("   ");  // Clear any leftover characters
}
