// The myo libray
#include <MyoController.h>

// Defining motor control pins, ultrasonic sensor pins & motor speeds 
#define motorLeftSpeed 5
#define motorLeftOnOff 6
#define motorRightOnOff 10
#define motorRightSpeed 11
#define trigPin 4
#define echoPin 3
#define motorVerySlow 50
#define motorSlow 70
#define motorMedium 90
#define motorFast 120
#define motorBrake 0

// Variables for ultrasonic sensor
long duration, inches, cm;

MyoController myo = MyoController();

// Init setup function
void setup(){
    Serial.begin(9600); // setup serial
    pinMode(motorLeftSpeed, OUTPUT);
    pinMode(motorLeftOnOff, OUTPUT);
    pinMode(motorRightSpeed, OUTPUT);
    pinMode(motorRightOnOff, OUTPUT);
    myo.initMyo();
}

// Loop function
void loop(){
    myo.updatePose();
    
    switch ( myo.getCurrentPose() ) {
    case rest:
        // Reset
        break;
    case fist:
        // Call forward function
        break;
    case waveIn:
        // Turn left function call here
        break;
    case waveOut:
        // Turn right function call here
        break;
    case fingersSpread:
        // Call the break function
        break;
    }
    
    delay(100);
    
    // Sending pulses & receive pulses on ultrasonic sensor
    sendPulse();
    rcvPulse();
    // Converting distance from ultrasonic sensor to cm
    convertToDistance();
    Serial.println(cm);
    delay(250);

    // If no object found within 100 cm then set motor speed to medium
    // Else if object found within 75 to 100 cm then set motor speed to slow
    // Else if object within 35 cm then break, reverse & turn left
    if (cm > 100){
        Forward(motorMedium);
    }
    else if (cm >= 75 && cm < 100){
        Forward(motorSlow);
    }
    else if (cm < 35){
        Brake(motorBrake);
        delay(250);
        Reverse(motorSlow);
        delay(200);
        TurnLeft(motorVerySlow);
        delay(35);
    }
}

// Function to propel motors forward
void Forward(int motorSpeed){
    Serial.println("Forward");
    delay(250);
    analogWrite(motorLeftSpeed, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to propel motors in reverse
void Reverse(int motorSpeed){
    Serial.println("Reverse");
    delay(250);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightOnOff, motorSpeed);
}

// Function to change the direction of the motors to turn left
void TurnLeft(int motorSpeed){
    Serial.println("Turn Right");
    delay(250);
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to change the direction of the motors to turn right
void TurnRight(int motorSpeed){
    Serial.println("Turn Left");
    delay(250);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorRightOnOff, motorSpeed);
    analogWrite(motorLeftSpeed, motorSpeed);
}

// Function to stop motors
void Brake(int motorSpeed){
    Serial.println("Brake");
    delay(125);
    analogWrite(motorLeftSpeed, motorBrake);
    analogWrite(motorRightSpeed, motorBrake);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
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
