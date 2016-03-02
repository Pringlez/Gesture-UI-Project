// The myo libray
#include <MyoController.h>

// Defining motor control pins, ultrasonic sensor pins & motor speeds 
#define motorLeftSpeed 5
#define motorLeftOnOff 6
#define motorRightOnOff 10
#define motorRightSpeed 11
#define trigPin 4
#define echoPin 3
#define motorVerySlow 70
#define motorSlow 90
#define motorMedium 110
#define motorFast 130
#define motorBrake 0

// Variables for ultrasonic sensor & current motor speed
long duration, inches, cm, motorSpeed;

// Creating new myo controller instance
MyoController myo = MyoController();

// Init setup function
void setup(){
    Serial.begin(9600);
    // Setting the motor control pins
    pinMode(motorLeftSpeed, OUTPUT);
    pinMode(motorLeftOnOff, OUTPUT);
    pinMode(motorRightSpeed, OUTPUT);
    pinMode(motorRightOnOff, OUTPUT);
    // Setting up myo instance
    myo.initMyo();
}

// Loop function
void loop(){
    delay(75);
    checkForObjects();
    getGesturePose();
    // If no objects found within specific distance then set motor speed
    if (cm > 50){
        motorSpeed = motorMedium;
    }
    else if(cm > 100){
        motorSpeed = motorFast;
    }
}

// Function to propel motors forward
void Forward(int motorSpeed){
    // If an object is found within 30cm then set motor speed to stop
    if (cm < 30){
        motorSpeed = motorBrake;
    }
    Serial.println("Forward");
    analogWrite(motorLeftSpeed, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to propel motors in reverse
void Reverse(int motorSpeed){
    Serial.println("Reverse");
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightOnOff, motorSpeed);
}

// Function to change the direction of the motors to turn left
void TurnLeft(int motorSpeed){
    Serial.println("Turn Right");
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to change the direction of the motors to turn right
void TurnRight(int motorSpeed){
    Serial.println("Turn Left");
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorRightOnOff, motorSpeed);
    analogWrite(motorLeftSpeed, motorSpeed);
}

// Function to stop motors
void Brake(int motorSpeed){
    Serial.println("Braking!");
    analogWrite(motorLeftSpeed, motorBrake);
    analogWrite(motorRightSpeed, motorBrake);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to get the current gesture pose
void getGesturePose(){
    myo.updatePose();
    // Getting current pose then apply specific motor function
    switch ( myo.getCurrentPose() ) {
      case rest:
          // Reset
          Brake(motorBrake);
          break;
      case fist:
          // Call forward function
          Forward(motorSpeed);
          break;
      case waveIn:
          // Turn left function call here
          TurnLeft(motorSlow);
          break;
      case waveOut:
          // Turn right function call here
          TurnRight(motorSlow);
          break;
      case fingersSpread:
          // Call the break function
          Reverse(motorSpeed);
          break;
    }
}

// Function to check for objects in front of the device
void checkForObjects(){
    // Sending pulses & receive pulses on ultrasonic sensor
    sendPulse();
    rcvPulse();
    // Converting distance from ultrasonic sensor to cm
    convertToDistance();
}

// Function to convert microseconds to inches
long microsecondsToInches(long microseconds){
    // According to Parallax's datasheet for the PING))), there are
    // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
    // second).  This gives the distance travelled by the ping, outbound
    // and return, so we divide by 2 to get the distance of the obstacle.
    // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
    return microseconds / 74 / 2;
}

// Function to convert microseconds to cm
long microsecondsToCentimeters(long microseconds){
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
}

// Function to send ultrasonic pulses
void sendPulse(){
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(35);
    digitalWrite(trigPin, LOW);
}

// Function to receive ultrasonic pulses
void rcvPulse(){
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
}

// Function to convert time into a distance
void convertToDistance(){
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    Serial.println(cm);
    delay(50);
}