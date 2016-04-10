# Gesture-UI-Project
## John Walsh & Arjun Kharel

PURPOSE OF THE APPLICATION
---------------
The purpose of this application is to demonstrate and implement various gesture controls. We have created a robot using an Arduino and controlled it using a Myo armband. We have also developed an android app that allows a user to control the movement of the robot via Bluetooth with the help of buttons presented in an app. The app also integrates the a speech recognition feature, which means, you could directly talk to your app and robot will react appropriately to your voice commands. 

![alt text](/images/gestureimg.PNG "Gestures")

Project Details
---------------
0. This project uses an Arduino Uno R3 as the main controller board. All other devices will interface with this board through the input / output pins to the 'ATmega328P' microcontroller.
0. A Myo armband will communicate over Bluetooth to the host PC through to the Arduino over a USB cable.
0. The motors are driven by a dedicated 'LM298N' driver board. The pins 5, 6, 9 and 10 will be connected from the Arduino to this motor driver.
0. There is also ultrasonic sensor, to be specific the 'HC-SR04' ultrasonic module. The sensor uses pin 3 and 4 on the Arduino to send and receive ultrasonic pulses that will detect any objects in front of the device.

Gesture Identified (Myo)
-----------------
Wave in (turn left)

Wave out (turn right)

Fist (forward)

Finger spread (back)

Building Process
----------------
The process of building the chassis was quite simple. A couple of nuts & bolts attached the motors & controller board. The motor driver board was mounted on the underside of the chassis. Wiring was tricky, cable management was nightmare but in the end we think it came together fairly well.

## Android App
The robot car can also be controlled by speech. A mobile app has been developed on the Android platform, link to the repository is [here.](https://github.com/ultimatecodelab/Bluetooth-Controller-Arduino). The user can speak directly to the mobile app with the phrases "Forward", "Back", "Left", "Right" and "Stop". The app will translate the speech to text and send the translated bytes to the Bluetooth chip. The Bluetooth chip will then communicate back to the Arduino to call the correct functions to control the motor driver which in turn will direct the dc motors.

## The Hardware
The chassis is based on 3mm thick sheet of plastic, all mounting holes were pre-drilled. There was just about enough space to mount a 3s LiPo battery, breadboard, motors + gearboxes, motor driver and microcontroller.

![alt text](/images/image_1.jpg "Chassis Image 1")

The underside of the chassis holds the breadboard into place along with most of the wiring & jumper cables. The motor driver is also visible.

![alt text](/images/image_2.jpg "Chassis Image 2")

Up close shot of the wiring on the breadboard

![alt text](/images/image_3.jpg "Chassis Image 3")

### Bluetooth Update
The Bluetooth chip has been added to extend the functionality of the car. All details about the Bluetooth module can be found [here.](https://www.adafruit.com/product/1697).

![alt text](/images/image_5.jpg "Chassis Image 4")

## The Software
First you need to upload the sketch to your Arduino board. Then run the application in the bin directory called 'MyoDuino.exe' to bind your Myo armband to your running Arduino.  When you run the executable you should see a window pop asking which COM port your Arduino device is using to communicate with your PC. Un-tick the checkbox also to stop the Myo armband from locking. You should see the different poses appearing in the terminal window. Also make sure your Myo armband is up to temperature before attempting to control the robot, as Myo only registers poses correctly when the armband is up to body temperature.

![alt text](/images/image_4.jpg "Software Image 1")


