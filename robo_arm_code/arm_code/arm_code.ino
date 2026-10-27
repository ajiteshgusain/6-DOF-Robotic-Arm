// Recommended Pre-Boot Configuration (The "Home" Pose)
// Match your physical arm joints as closely as possible to these starting angles before powering it on:

// Base Rotation (Channel 0 - MG996R): Centered facing straight ahead (~90°).

// Shoulder Lift (Channel 1 - MG996R): Lifted upright or partially raised (~150°).

// Elbow Movement (Channel 2 - MG996R): Moderately bent (~35°).

// Wrist & Gripper (Channels 3 to 5 - SG90s): Set to their neutral, middle ranges (~140°, 85°, 80°).


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BluetoothSerial.h"

// Check if Bluetooth is enabled in your ESP32 board settings
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please check your ESP32 board configuration.
#endif

BluetoothSerial Bluetooth;                                      // ESP32 Native Bluetooth Serial
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);    // PCA9685 default I2C address 0x40

#define SERVOMIN  125                                           // Minimum pulse length count for 0° (adjust if needed)
#define SERVOMAX  625                                           // Maximum pulse length count for 180° (adjust if needed)

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // current position
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // previous position
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50], servo05SP[50], servo06SP[50]; // for storing positions/steps
int speedDelay = 20;
int index = 0;
String dataIn = "";

// Helper function to map degrees (0-180) to PCA9685 12-bit tick counts
int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}

void setup() {
  Serial.begin(115200);

  // Initialize I2C and the PCA9685 module
  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz

  // Initialize ESP32 Bluetooth (Name shown on your phone's Bluetooth menu)
  Bluetooth.begin("ESP32_Robot_Arm");
  Serial.println("Bluetooth is ready! Pair your phone to 'ESP32_Robot_Arm'.");

  delay(20);

  // Robot arm initial positions (Channels 0 to 5)
  servo1PPos = 90;
  pwm.setPWM(0, 0, angleToPulse(servo1PPos));
  
  servo2PPos = 150;
  pwm.setPWM(1, 0, angleToPulse(servo2PPos));
  
  servo3PPos = 35;
  pwm.setPWM(2, 0, angleToPulse(servo3PPos));
  
  servo4PPos = 140;
  pwm.setPWM(3, 0, angleToPulse(servo4PPos));
  
  servo5PPos = 85;
  pwm.setPWM(4, 0, angleToPulse(servo5PPos));
  
  servo6PPos = 80;
  pwm.setPWM(5, 0, angleToPulse(servo6PPos));
}

void loop() {
  // Check for incoming data from the Bluetooth app
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();  
    dataIn.trim(); // Remove unwanted trailing spaces or newlines
    
    // Move Servo 1 (Base - Channel 0)
    if (dataIn.startsWith("s1")) {
      String dataInS = dataIn.substring(2); 
      servo1Pos = dataInS.toInt();  
      if (servo1PPos > servo1Pos) {
        for (int j = servo1PPos; j >= servo1Pos; j--) {   
          pwm.setPWM(0, 0, angleToPulse(j));
          delay(20);    
        }
      }
      if (servo1PPos < servo1Pos) {
        for (int j = servo1PPos; j <= servo1Pos; j++) {   
          pwm.setPWM(0, 0, angleToPulse(j));
          delay(20);
        }
      }
      servo1PPos = servo1Pos;   
    }
    
    // Move Servo 2 (Shoulder - Channel 1)
    if (dataIn.startsWith("s2")) {
      String dataInS = dataIn.substring(2);
      servo2Pos = dataInS.toInt();
      if (servo2PPos > servo2Pos) {
        for (int j = servo2PPos; j >= servo2Pos; j--) {
          pwm.setPWM(1, 0, angleToPulse(j));
          delay(50);
        }
      }
      if (servo2PPos < servo2Pos) {
        for (int j = servo2PPos; j <= servo2Pos; j++) {
          pwm.setPWM(1, 0, angleToPulse(j));
          delay(50);
        }
      }
      servo2PPos = servo2Pos;
    }
    
    // Move Servo 3 (Elbow - Channel 2)
    if (dataIn.startsWith("s3")) {
      String dataInS = dataIn.substring(2);
      servo3Pos = dataInS.toInt();
      if (servo3PPos > servo3Pos) {
        for (int j = servo3PPos; j >= servo3Pos; j--) {
          pwm.setPWM(2, 0, angleToPulse(j));
          delay(30);
        }
      }
      if (servo3PPos < servo3Pos) {
        for (int j = servo3PPos; j <= servo3Pos; j++) {
          pwm.setPWM(2, 0, angleToPulse(j));
          delay(30);
        }
      }
      servo3PPos = servo3Pos;
    }
    
    // Move Servo 4 (Wrist Pitch - Channel 3)
    if (dataIn.startsWith("s4")) {
      String dataInS = dataIn.substring(2);
      servo4Pos = dataInS.toInt();
      if (servo4PPos > servo4Pos) {
        for (int j = servo4PPos; j >= servo4Pos; j--) {
          pwm.setPWM(3, 0, angleToPulse(j));
          delay(30);
        }
      }
      if (servo4PPos < servo4Pos) {
        for (int j = servo4PPos; j <= servo4Pos; j++) {
          pwm.setPWM(3, 0, angleToPulse(j));
          delay(30);
        }
      }
      servo4PPos = servo4Pos;
    }
    
    // Move Servo 5 (Gripper - Channel 4)
    if (dataIn.startsWith("s5")) {
      String dataInS = dataIn.substring(2);
      servo5Pos = dataInS.toInt();
      if (servo5PPos > servo5Pos) {
        for (int j = servo5PPos; j >= servo5Pos; j--) {
          pwm.setPWM(4, 0, angleToPulse(j));
          delay(30);
        }
      }
      if (servo5PPos < servo5Pos) {
        for (int j = servo5PPos; j <= servo5Pos; j++) {
          pwm.setPWM(4, 0, angleToPulse(j));
          delay(30);
        }
      }
      servo5PPos = servo5Pos;
    }
    
    // Move Servo 6 (Spare/Extra Joint - Channel 5)
    if (dataIn.startsWith("s6")) {
      String dataInS = dataIn.substring(2);
      servo6Pos = dataInS.toInt();
      if (servo6PPos > servo6Pos) {
        for (int j = servo6PPos; j >= servo6Pos; j--) {
          pwm.setPWM(5, 0, angleToPulse(j));
          delay(30);
        }
      }
      if (servo6PPos < servo6Pos) {
        for (int j = servo6PPos; j <= servo6Pos; j++) {
          pwm.setPWM(5, 0, angleToPulse(j));
          delay(30);
        }
      }
      servo6PPos = servo6Pos; 
    }
    
    // If button "SAVE" is pressed in the app
    if (dataIn.startsWith("SAVE")) {
      servo01SP[index] = servo1PPos;  // Save current positions into array
      servo02SP[index] = servo2PPos;
      servo03SP[index] = servo3PPos;
      servo04SP[index] = servo4PPos;
      servo05SP[index] = servo5PPos;
      servo06SP[index] = servo6PPos;
      index++;                        // Increase array index
    }
    
    // If button "RUN" is pressed in the app
    if (dataIn.startsWith("RUN")) {
      runservo();  // Run automatic mode loop
    }
    
    // If button "RESET" is pressed in the app
    if (dataIn == "RESET") {
      memset(servo01SP, 0, sizeof(servo01SP)); // Clear all saved steps
      memset(servo02SP, 0, sizeof(servo02SP));
      memset(servo03SP, 0, sizeof(servo03SP));
      memset(servo04SP, 0, sizeof(servo04SP));
      memset(servo05SP, 0, sizeof(servo05SP));
      memset(servo06SP, 0, sizeof(servo06SP));
      index = 0;  // Reset index to 0
    }
  }
}

// Automatic sequence replay mode function
void runservo() {
  while (dataIn != "RESET") {   
    for (int i = 0; i <= index - 2; i++) {  
      if (Bluetooth.available() > 0) {      
        dataIn = Bluetooth.readString();
        dataIn.trim();
        if (dataIn == "PAUSE") {           
          while (dataIn != "RUN") {         
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.readString();
              dataIn.trim();
              if (dataIn == "RESET") {     
                break;
              }
            }
          }
        }
        if (dataIn.startsWith("ss")) {
          String dataInS = dataIn.substring(2);
          speedDelay = dataInS.toInt(); // Update speed delay value
        }
      }
      
      // Servo 1 (Channel 0)
      if (servo01SP[i] > servo01SP[i + 1]) {
        for (int j = servo01SP[i]; j >= servo01SP[i + 1]; j--) {
          pwm.setPWM(0, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo01SP[i] < servo01SP[i + 1]) {
        for (int j = servo01SP[i]; j <= servo01SP[i + 1]; j++) {
          pwm.setPWM(0, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }

      // Servo 2 (Channel 1)
      if (servo02SP[i] > servo02SP[i + 1]) {
        for (int j = servo02SP[i]; j >= servo02SP[i + 1]; j--) {
          pwm.setPWM(1, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo02SP[i] < servo02SP[i + 1]) {
        for (int j = servo02SP[i]; j <= servo02SP[i + 1]; j++) {
          pwm.setPWM(1, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }

      // Servo 3 (Channel 2)
      if (servo03SP[i] > servo03SP[i + 1]) {
        for (int j = servo03SP[i]; j >= servo03SP[i + 1]; j--) {
          pwm.setPWM(2, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo03SP[i] < servo03SP[i + 1]) {
        for (int j = servo03SP[i]; j <= servo03SP[i + 1]; j++) {
          pwm.setPWM(2, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }

      // Servo 4 (Channel 3)
      if (servo04SP[i] > servo04SP[i + 1]) {
        for (int j = servo04SP[i]; j >= servo04SP[i + 1]; j--) {
          pwm.setPWM(3, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo04SP[i] < servo04SP[i + 1]) {
        for (int j = servo04SP[i]; j <= servo04SP[i + 1]; j++) {
          pwm.setPWM(3, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }

      // Servo 5 (Channel 4)
      if (servo05SP[i] > servo05SP[i + 1]) {
        for (int j = servo05SP[i]; j >= servo05SP[i + 1]; j--) {
          pwm.setPWM(4, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo05SP[i] < servo05SP[i + 1]) {
        for (int j = servo05SP[i]; j <= servo05SP[i + 1]; j++) {
          pwm.setPWM(4, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }

      // Servo 6 (Channel 5)
      if (servo06SP[i] > servo06SP[i + 1]) {
        for (int j = servo06SP[i]; j >= servo06SP[i + 1]; j--) {
          pwm.setPWM(5, 0, angleToPulse(j));
          delay(speedDelay);
        }
      } else if (servo06SP[i] < servo06SP[i + 1]) {
        for (int j = servo06SP[i]; j <= servo06SP[i + 1]; j++) {
          pwm.setPWM(5, 0, angleToPulse(j));
          delay(speedDelay);
        }
      }
    }
  }
}