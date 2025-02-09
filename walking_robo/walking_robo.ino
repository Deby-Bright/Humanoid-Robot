#include <Wire.h>                   // Library for I2C communication
#include <Adafruit_PWMServoDriver.h> // Library to control PCA9685 servo driver

// Create a PCA9685 object to control the servo motors
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Constants for servo pulse ranges
#define SERVOMIN 150  // Minimum pulse length count (corresponds to 0°)
#define SERVOMAX 600  // Maximum pulse length count (corresponds to 180°)

// Servo channels for the robot's legs (based on your description)
// Human POV (Front View):
// 0: Left Leg, 1: Left Thigh, 2: Right Leg, 3: Right Thigh
// Robot POV (Internal Mapping):
// 0: Right Leg, 1: Right Thigh, 2: Left Leg, 3: Left Thigh
const int servoChannels[4] = {0, 1, 2, 3};

// Array to store the current angles of all servos
// All servos start at 90° (neutral position)
int servoAngles[4] = {90, 90, 90, 90};

// Function to map servo angle to pulse width
// The PCA9685 requires a pulse between SERVOMIN and SERVOMAX for 0° to 180°
int angleToPulse(int angle) {
  return map(angle, 0, 100, SERVOMIN, SERVOMAX);
}

// Function to move a servo to a specific angle with smooth motion
// channel: The servo channel on the PCA9685 board
// targetAngle: The desired angle for the servo
// stepDelay: Delay between steps for smoothness (default: 10ms)
void setServoAngle(int channel, int targetAngle, int stepDelay = 10) {
  int currentAngle = servoAngles[channel]; // Get the current angle of the servo

  // Determine the direction of movement
  int step = (targetAngle > currentAngle) ? 1 : -1;

  // Move the servo in small increments to create a smooth motion
  for (int angle = currentAngle; angle != targetAngle; angle += step) {
    pwm.setPWM(channel, 0, angleToPulse(angle)); // Send the pulse to the servo
    delay(stepDelay); // Delay between each step for smooth motion
  }

  // Update the stored angle to the new target
  servoAngles[channel] = targetAngle;
}

// Function to initialize all servos to their neutral positions (90°)
void initializeServos() {
  for (int i = 0; i < 4; i++) { // Loop through all 4 servo channels
    setServoAngle(i, 90);      // Set each servo to its neutral position
  }
}

// Function to simulate a penguin-like walking motion
void walk() {
  // Step 1: Lift the left leg and tilt the left thigh outward
  setServoAngle(2, 100); // Lift left leg
  setServoAngle(3, 60);  // Tilt left thigh outward for balance
  delay(500);            // Pause for stability

  // Step 2: Swing the left leg forward
  setServoAngle(2, 60);  // Move left leg forward
  setServoAngle(3, 90);  // Bring left thigh back to neutral
  delay(500);

  // Step 3: Return the left leg to its neutral position
  setServoAngle(2, 90);  // Reset left leg to neutral
  delay(500);

  // Step 4: Lift the right leg and tilt the right thigh outward
  setServoAngle(0, 100); // Lift right leg
  setServoAngle(1, 60);  // Tilt right thigh outward for balance
  delay(500);

  // Step 5: Swing the right leg forward
  setServoAngle(0, 60);  // Move right leg forward
  setServoAngle(1, 90);  // Bring right thigh back to neutral
  delay(500);

  // Step 6: Return the right leg to its neutral position
  setServoAngle(0, 90);  // Reset right leg to neutral
  delay(500);

  // Optional: Add torso swing for a more natural penguin-like motion
  setServoAngle(1, 100); // Tilt torso slightly to the right
  delay(300);
  setServoAngle(3, 100); // Tilt torso slightly to the left
  delay(300);

  // Reset torso to neutral position
  setServoAngle(1, 90);
  setServoAngle(3, 90);
  delay(300);
}

void setup() {
  Serial.begin(9600); // Initialize Serial communication for debugging
  Serial.println("Initializing PCA9685...");

  pwm.begin();        // Initialize the PCA9685
  pwm.setPWMFreq(60); // Set the PWM frequency to 60 Hz (suitable for servos)

  initializeServos(); // Move all servos to their neutral positions
  delay(1000);        // Wait for the servos to stabilize
}

void loop() {
  walk(); // Execute the walking motion repeatedly
}