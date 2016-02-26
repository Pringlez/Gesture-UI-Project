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

long duration, inches, cm;

void setup(){
    Serial.begin(9600); // setup serial
    pinMode(motorLeftSpeed, OUTPUT);
    pinMode(motorLeftOnOff, OUTPUT);
    pinMode(motorRightSpeed, OUTPUT);
    pinMode(motorRightOnOff, OUTPUT);
}

void loop(){
    sendPulse();
    rcvPulse();
    convertToDistance();
    Serial.println(cm);
    delay(250);
    
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

void Forward(int motorSpeed){
    Serial.println("Forward");
    delay(250);
    analogWrite(motorLeftSpeed, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

void Reverse(int motorSpeed){
    Serial.println("Reverse");
    delay(250);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightOnOff, motorSpeed);
}

void TurnLeft(int motorSpeed){
    Serial.println("Turn Right");
    delay(250);
    analogWrite(motorLeftOnOff, motorSpeed);
    analogWrite(motorRightSpeed, motorSpeed);
    digitalWrite(motorLeftSpeed, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

void TurnRight(int motorSpeed){
    Serial.println("Turn Left");
    delay(250);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightSpeed, LOW);
    analogWrite(motorRightOnOff, motorSpeed);
    analogWrite(motorLeftSpeed, motorSpeed);
}

void Brake(int motorSpeed){
    Serial.println("Brake");
    delay(125);
    analogWrite(motorLeftSpeed, motorBrake);
    analogWrite(motorRightSpeed, motorBrake);
    digitalWrite(motorLeftOnOff, LOW);
    digitalWrite(motorRightOnOff, LOW);
}

long microsecondsToInches(long microseconds){
    // According to Parallax's datasheet for the PING))), there are
    // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
    // second).  This gives the distance travelled by the ping, outbound
    // and return, so we divide by 2 to get the distance of the obstacle.
    // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
    return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds){
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
}

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

void rcvPulse(){
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
}

void convertToDistance(){
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    Serial.println(cm);
    delay(50);
}

