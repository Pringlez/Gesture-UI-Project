# Gesture-UI-Project
## John Walsh & Arjun Kharel

Project Details
---------------
0. The project uses an Arduino Uno R3 as the main controller board. All other devices will interface with this board through the input / output pins to the 'ATmega328P' microcontroller.
0. The Myo armband will communicate over bluetooth to the host PC through to the Arduino over a USB cable.
0. The motors are driven by a dedicated 'LM298N' driver board. The pins 5, 6, 10 and 11 will be connected from the Arduino to this motor driver.
0. The project also employs the use of a ultrasonic sensor, to be specific the 'HC-SR04' ultrasonic module. The sensor uses pin 3 and 4 on the Arduino to send and receive ultrasonic pulses.

Building Process
----------------
The process of building the chassis was quite simple. A couple of nuts & bolts attached the motors & controller board. The motor driver board was mounted on the under side of the chassis. Wiring was tricky, cable management was nightmare but in the end we think it came together fairly well.

##Mobile App
https://github.com/ultimatecodelab/Bluetooth-Controller-Arduino
We hope to control our robot using  speech. Mobile app will be programmed in Java. User can speak the phrases like "move forward / back". The app will translate the speech to text and send  the translated bytes to bluetooth chip. Bluetooth chip will be programmed to listen for any incoming messages.

## The Hardware
The chassis is based on 3mm thick sheet of plastic, all mounting holes were pre-drilled. There was just about enough space to mount a 3s LiPo battery, breadboard, motor driver and microcontroller.

![alt text](/images/image_1.jpg "Chassis Image 1")

The underside of the chassis holds the breadboard into place along with most of the wiring & jumper cables. The motor driver is also visible.

![alt text](/images/image_2.jpg "Chassis Image 2")

## The Software
The software is a currently under development, a basic version of the software exists in the repository. The Myo armband gesture have not been implemented yet.

To be updated...
