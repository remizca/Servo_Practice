Here’s the code to get your project running with the Arduino Uno R3. The code includes functionality for detecting sound, controlling the servos to move toward the sound, displaying the detection count and ongoing detection status with a blinking square on the OLED display.

Required Libraries:
Servo Library (built-in with Arduino IDE)
Adafruit SSD1306 for the OLED display.
Adafruit GFX for drawing on the OLED.
Wire Library (for I2C communication, typically pre-installed).
You can install the Adafruit SSD1306 and Adafruit GFX libraries via the Library Manager in the Arduino IDE.

Explanation of Code:

Sound Detection:
The sound sensor is connected to A0, and we read its analog value. If the value exceeds a certain threshold, it counts as a sound detection. The detection count is incremented only when a new sound is detected (i.e., the sensor wasn’t detecting sound just before).

Servo Movement:
We map the sound sensor value to the movement of two servos (horizontal and vertical) to "turn" the machine toward the sound. This basic mapping could be fine-tuned for more accurate direction detection.

OLED Display:
It displays the total number of sound detections.
If a sound is ongoing, a blinking square appears in the top-left corner.
When sound is detected, the display shows "Sound Detected" below the counter.

Blinking Logic:
The blinking square is toggled every 500 ms to indicate ongoing sound detection.
