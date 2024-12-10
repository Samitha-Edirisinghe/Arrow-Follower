#include <Wire.h>
#include <AFMotor.h>

// Motor driver instance
AF_DCMotor motor1(1); // Motor 1 connected to M1
AF_DCMotor motor2(2); // Motor 2 connected to M2
AF_DCMotor motor3(3); // Motor 3 connected to M3
AF_DCMotor motor4(4); // Motor 4 connected to M4

void setup() {
  Serial.begin(9600); // Initialize serial communication for Bluetooth
  Wire.begin();       // Initialize I2C communication
  cameraInit();       // Initialize the camera
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the command from the serial port

    switch (command) {
      case 'L': // Turn left
        turnLeft();
        break;
      case 'R': // Turn right
        turnRight();
        break;
      case 'F': // Move forward
        moveForward();
        break;
      case 'B': // Move backward
        moveBackward();
        break;
      case 'C': // Capture image
        captureImage();
        break;
      default:
        stopMotors();
        break;
    }
  }
}

void cameraInit() {
  // Send initialization commands to the camera
  Wire.beginTransmission(0x42); // Replace 0x42 with your camera's I2C address
  Wire.write(0x12);             // Example command, replace with actual initialization sequence
  Wire.endTransmission();
}

void captureImage() {
  Wire.beginTransmission(0x42); // Replace 0x42 with your camera's I2C address
  Wire.write(0x00);             // Command to capture image
  Wire.endTransmission();
  
  delay(100); // Wait for image capture
  
  Wire.requestFrom(0x42, 8); // Request 8 bytes of data (for example)
  while (Wire.available()) {
    char c = Wire.read();
    Serial.write(c); // Send the captured data to the computer via Bluetooth
  }
}

void moveForward() {
  motor1.setSpeed(255); // Set speed to maximum
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.setSpeed(255); // Set speed to maximum
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft() {
  motor1.setSpeed(255); // Set speed to maximum
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnRight() {
  motor1.setSpeed(255); // Set speed to maximum
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void stopMotors() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
