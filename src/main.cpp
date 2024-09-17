#include <Arduino.h>
#include <Servo.h>  // Include the Servo library

Servo myservo;  // Create a servo object

const int buttonPin1 = 2;  // Pin for the first button
const int buttonPin2 = 3;  // Pin for the second button
int angle = 90;  // Initial angle for the servo

/**
 * @brief Initialize the servo and the two buttons.
 *
 * Initialize the servo to the initial angle (90 degrees) and set the two
 * buttons as input with internal pull-up resistors.
 */
void setup() {
  myservo.attach(9);  // Attaches the servo on pin 9 to the servo object
  myservo.write(angle);  // Initialize servo to 90 degrees
  pinMode(buttonPin1, INPUT_PULLUP);  // Set button pin 1 as input with internal pull-up resistor
  pinMode(buttonPin2, INPUT_PULLUP);  // Set button pin 2 as input with internal pull-up resistor
}


/**
 * @brief Main loop of the program.
 *
 * Check the state of the two buttons and adjust the servo angle accordingly.
 * If button 1 is pressed, increase the angle by 1 degree. If button 2 is
 * pressed, decrease the angle by 1 degree. The angle is limited to the range
 * 0 to 180 degrees. The servo is moved to the new angle and a short delay is
 * used to allow the servo to reach the new position.
 */

void loop() {
    static int8_t dir = 0;
    if (digitalRead(buttonPin1) == LOW) dir = 1;
    if (digitalRead(buttonPin2) == LOW) dir = -1;
    
    if (dir != 0) {
      angle = constrain(angle + dir, 0, 180);
      myservo.write(angle);
      delay(15);
      dir = 0;
    }
  }
  


