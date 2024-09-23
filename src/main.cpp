#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin definitions
const int soundSensorPin = A0;
const int threshold = 400; // Set the threshold for sound detection
const int horizontalServoPin = 9;
const int verticalServoPin = 10;

Servo horizontalServo;
Servo verticalServo;

int soundDetectionCount = 0;
bool ongoingSound = false;
int blinkState = 0;
unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 500; // Blink every 500ms

void updateDisplay(bool isDetecting);

void setup()
{
  // Initialize servos
  horizontalServo.attach(horizontalServoPin);
  verticalServo.attach(verticalServoPin);
  horizontalServo.write(90); // Start in center position (left-right)
  verticalServo.write(90);   // Start in center position (up-down)

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();

  // Initialize Serial Monitor
  Serial.begin(9600);
}

void loop()
{
  // Read sound sensor value
  int soundValue = analogRead(soundSensorPin);

  if (soundValue > threshold)
  {
    // Sound detected
    if (!ongoingSound)
    {
      // Count only when a new sound starts
      soundDetectionCount++;
      ongoingSound = true;
    }
    Serial.println("Sound detected!");

    // Turn toward sound (This is basic. You may improve direction detection.)
    horizontalServo.write(map(soundValue, threshold, 1023, 45, 135)); // Simple mapping to servo angles
    verticalServo.write(map(soundValue, threshold, 1023, 45, 135));   // Can adjust to your preference

    // Display detection info on OLED
    updateDisplay(true);
  }
  else
  {
    // No sound detected
    ongoingSound = false;
    updateDisplay(false);
  }

  // Blink square if sound is ongoing
  if (ongoingSound)
  {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= blinkInterval)
    {
      blinkState = !blinkState;
      lastBlinkTime = currentTime;
    }
  }
  else
  {
    blinkState = 0;
  }
}

void updateDisplay(bool isDetecting)
{
  display.clearDisplay();

  // Display sound detection count
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Count: ");
  display.println(soundDetectionCount);

  // Display ongoing detection status
  if (isDetecting)
  {
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Sound Detected");
  }

  // Display blinking square in top-left corner
  if (blinkState)
  {
    display.fillRect(0, 0, 10, 10, SSD1306_WHITE); // Draw a 10x10 square
  }

  display.display();
}
