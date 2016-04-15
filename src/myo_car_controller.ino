// The myo libray & bluetooth
#include <MyoController.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// Bluetooth control pins
// UNO : CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 7
#define ADAFRUITBLE_REQ 8

// Defining motor control pins, ultrasonic sensor pins & motor speeds 
#define motorLeftSpeed 5
#define motorLeftOnOff 6
#define motorRightOnOff 9
#define motorRightSpeed 10
#define echoPin 3
#define trigPin 4
#define motorVerySlow 70
#define motorSlow 90
#define motorMedium 110
#define motorFast 130
#define motorBrake 0

// Variables for ultrasonic sensor & current motor speed
long duration, inches, cm, bluetoothMode;

// Creating new myo controller & bluetooth instance
MyoController myo = MyoController();
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

// Init setup function
void setup(){
    // Setting the motor control pins
    pinMode(motorLeftSpeed, OUTPUT);
    pinMode(motorLeftOnOff, OUTPUT);
    pinMode(motorRightSpeed, OUTPUT);
    pinMode(motorRightOnOff, OUTPUT);
    Serial.begin(9600);
    while(!Serial);
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 - Running!"));
    // Setting name, max seven characters for bluetooth device name!
    BTLEserial.setDeviceName("MyoCar");
    BTLEserial.begin();
    // Setting up myo instance
    bluetoothMode = false;
    myo.initMyo();
}

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

// Loop function
void loop(){
    delay(75);
    checkForObjects();
    if(bluetoothMode){
      checkBluetooth();
    }
    else{
      getGesturePose();
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
    Serial.println("Turn Left");
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

// Function to change the direction of the motors to turn right
void TurnRight(int motorSpeed){
    Serial.println("Turn Right");
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

// Function to check bluetooth status
void checkBluetooth(){
    BTLEserial.pollACI();
    // What is current status of chip
    aci_evt_opcode_t status = BTLEserial.getState();
    // If the status changed....
    if (status != laststatus) {
        // print it out!
        if (status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Advertising started"));
        }
        if (status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connected!"));
        }
        if (status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Disconnected or advertising timed out"));
        }
        // OK set the last status change to this one
        laststatus = status;
    }

    if (status == ACI_EVT_CONNECTED) {
        // Lets see if there's any data for us!
        if (BTLEserial.available()) {
            Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
        }
        // OK while we still have something to read, get a character and print it out
        while (BTLEserial.available()) {
            char cmd = BTLEserial.read();
            getBluetoothCmd(toupper(cmd));
        }
    }

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
        // Read a line from Serial
        Serial.setTimeout(100); // 100 millisecond timeout
        String s = Serial.readString();
        
        // We need to convert the line to bytes, no more than 20 at this time
        uint8_t sendbuffer[20];
        s.getBytes(sendbuffer, 20);
        char sendbuffersize = min(20, s.length());
        
        Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");
        
        // write the data
        BTLEserial.write(sendbuffer, sendbuffersize);
    }
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
          Forward(motorSlow);
          break;
      case waveIn:
          // Turn left function call here
          TurnLeft(motorVerySlow);
          break;
      case waveOut:
          // Turn right function call here
          TurnRight(motorVerySlow);
          break;
      case fingersSpread:
          // Call the break function
          Reverse(motorSlow);
          break;
    }
}

// Function to get command from bluetooth
void getBluetoothCmd(char command){
    // Getting current pose then apply specific motor function
    switch ( command ) {
      case 'N':
          // Reset
          Brake(motorBrake);
          break;
      case 'F':
          // Call forward function
          Forward(motorSlow);
          break;
      case 'L':
          // Turn left function call here
          TurnLeft(motorSlow);
          break;
      case 'R':
          // Turn right function call here
          TurnRight(motorVerySlow);
          break;
      case 'B':
          // Call the break function
          Reverse(motorVerySlow);
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
    //Serial.println(cm);
    delay(50);
}