#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo channels
const int servoChannels[7] = {0, 1, 2, 3, 4, 5, 6};
// Motor names
String motorNames[7] = {"m0", "m1", "m2", "m3", "m4", "m5", "m6"};

// Pulse length constants
int SERVOMIN = 150;  // Minimum pulse length
int SERVOMAX = 600;  // Maximum pulse length

// Current target angles for the servos
int targetAngle[7] = {90, 90, 90, 90, 90, 90, 90};

// Function to set a servo to a specific angle
void setServoAngle(int channel, int targetAngle) {
  int pulse = map(targetAngle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(channel, 0, pulse);
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Set frequency to 60 Hz
  Serial.println("Setup complete. Ready for commands.");
}

void loop() {
  String command = "";

  // Check for incoming commands via serial
  if (Serial.available() > 0) {
    command = Serial.readString();
    Serial.print("Received Command: ");
    Serial.println(command);

    // Check if the command starts with "set"
    if (command.substring(0, 3) == "set") {
      // Parse the motor number and target angle
      int motorNum = command.substring(4, 5).toInt();
      int newAngle = command.substring(6).toInt();

      // Update the target angle and move the servo
      if (motorNum >= 0 && motorNum < 7) {
        targetAngle[motorNum] = newAngle;
        setServoAngle(servoChannels[motorNum], targetAngle[motorNum]);
        Serial.print("Motor ");
        Serial.print(motorNames[motorNum]);
        Serial.print(" set to ");
        Serial.println(targetAngle[motorNum]);
      } else {
        Serial.println("Invalid motor number!");
      }
    } else {
      Serial.println("Invalid command format!");
    }
  }

  delay(100);  // Add a small delay to avoid overloading the loop
}