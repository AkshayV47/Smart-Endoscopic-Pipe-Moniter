#include <Servo.h>

Servo servoX;  // Servo for X-axis (first rod segment, pin D9)
Servo servoY;  // Servo for Y-axis (second rod segment, pin D11)

const int joyX = A0;  // Joystick X-axis
const int joyY = A2;  // Joystick Y-axis
const int joySW = 2;  // Joystick button (optional)

const int deadzone = 50;  // Deadzone around center to avoid jitter

void setup() {
  servoX.attach(10);   // Attach servo to D9 (X-axis, first segment)
  servoY.attach(8);  // Attach servo to D11 (Y-axis, second segment)
  
  pinMode(joySW, INPUT_PULLUP);  // Enable pull-up for button (optional)
  
  Serial.begin(9600);  // For debugging
  Serial.println("Joystick Servo Control Started");
  
  // Set initial positions: X at 180°, Y at 0°
  servoX.write(180);
  servoY.write(0);
}

void loop() {
  // Read joystick values (0 to 1023)
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);
  int buttonState = digitalRead(joySW);  // Optional: 0 if pressed
  
  // Map joystick values to servo angles
  // X: 0 (left) -> 0°, 512 (center) -> 180°, 1023 (right) -> 360° (capped at 180°)
  // Y: 0 (down) -> 180°, 512 (center) -> 0°, 1023 (up) -> -180° (capped at 0°)
  int xAngle = map(xValue, 0, 1023, 0, 360);  // Extended range for smooth mapping
  int yAngle = map(yValue, 0, 1023, 180, -180);  // Inverted for Y
  
  // Apply deadzone: If near center (512 ± deadzone), set to neutral (X: 180°, Y: 0°)
  if (abs(xValue - 512) < deadzone) {
    xAngle = 180;  // Neutral position for Servo X
  }
  if (abs(yValue - 512) < deadzone) {
    yAngle = 0;    // Neutral position for Servo Y
  }
  
  // Constrain angles to servo limits (0° to 180°)
  xAngle = constrain(xAngle, 0, 180);
  yAngle = constrain(yAngle, 0, 180);
  
  // Write angles to servos
  servoX.write(xAngle);  // Controls first rod segment
  servoY.write(yAngle);  // Controls second rod segment
  
  // Debugging output
  Serial.print("X Value: ");
  Serial.print(xValue);
  Serial.print(" | X Angle: ");
  Serial.print(xAngle);
  Serial.print(" | Y Value: ");
  Serial.print(yValue);
  Serial.print(" | Y Angle: ");
  Serial.print(yAngle);
  Serial.print(" | Button: ");
  Serial.println(buttonState);
  
  delay(50);  // Small delay for smooth control
}