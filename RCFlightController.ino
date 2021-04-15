/*
*
*               RC Flight Controller Sketch
*                   By insertZCodeHere
*
*/

#include <nRF24L01.h>
#include <RF24.h>
// Library for Radio Communication https://www.arduino.cc/reference/en/libraries/rf24/

#include <SPI.h>
// Library for Enabling communication with another arduino https://www.arduino.cc/en/reference/SPI

#include <AccelStepper.h>
// Library for controlling one or more stepper motors https://www.arduino.cc/reference/en/libraries/accelstepper/

#include <Servo.h>
// Library for controlling servos https://www.arduino.cc/reference/en/libraries/servo/


// Pins on the arduino and the components they're connected to
int RadioMOSI = 11;
int RadioMISO = 12;
int RadioSCK = 13;
int RadioCSN = 10;
int RadioCE = 8;
int ServoL = 2;
int ServoR = 3;
int Step1 = 4;
int Step2 = 5;
int Step3 = 6;
int Step4 = 7;


// Variables relating to components
Servo leftAileron;
Servo rightAileron;
AccelStepper propeller(4, Step1, Step2, Step3, Step4);
RF24 receiver(RadioCE, RadioCNE);           // Creates a RF24 object passing the number of the CE and CNE pins
const address[5] = "12345";                 // Uniquely identifying address for the transmitter to know
int leftAileronVal;
int rightAileronVal;
int propellerVal;
bool ontoggle


//Data structure sent by transmitter
struct packet {
    int xpos;
    int ypos;
    bool stick;
}
packet data;

void setup() {
    serial.begin(9600);                     // Baudrate of 9600
    leftAileron.attach(ServoL);
    rightAileron.attach(ServoR);
    propeller.setMaxSpeed(1000);
    propeller.setAcceleration(100);
    receiver.openReadingPipe(1, address);   // Configure the radio as a receiver on pipe 1
    receiver.setAutoAck(1, false);          // Disable auto acknowledgement response when receiving a packet
    receiver.setDataRate(RF24_250KBPS);     // Minimum data rate
    receiver.setPALevel(RF24_PA_MAX, 1);    // Maximum power amplification + LNA enabled 
    receiver.startListening();
}

void loop() {
    // Write the main loop
        // First checks if the controller is on and data has been set
        // Then maps and passes these values to different channels
        // Repeat
}