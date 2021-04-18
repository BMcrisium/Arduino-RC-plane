/*
*
*               RC Flight Controller Sketch
*                   By Z0161166
*
*/

#include <nRF24L01.h>
#include <RF24.h>
// Library for Radio Communication https://www.arduino.cc/reference/en/libraries/rf24/

#include <SPI.h>
// Library for Enabling communication with another arduino https://www.arduino.cc/en/reference/SPI

#include <Servo.h>
// Library for controlling servos and brushless motor https://www.arduino.cc/reference/en/libraries/servo/


// Pins on the arduino and the components they're connected to
int RadioMOSI = 11;
int RadioMISO = 12;
int RadioSCK = 13;
int RadioCSN = 10;
int RadioCE = 8;
int RadioIRQ = 3;
int ServoL = 4;
int ServoR = 5;
int Prop = 6;

//Timing variables
unsigned long currentTime;
unsigned long lastTime;
unsigned long lastToggle;

// Variables relating to components
Servo leftAileron;
Servo rightAileron;
Servo propeller;
RF24 receiver(RadioCE, RadioCSN);           // Creates a RF24 object passing the number of the CE and CNE pins
const byte address[6] = "00111";            // Uniquely identifying address to link with transmitter
int leftAileronVal;
int rightAileronVal;
int propellerVal;
bool ontoggle;


//Data structure sent by transmitter
struct packet {
    
    int xpos;
    int ypos;
    bool stick;
};

packet data;                                // Create an instance of the packet data structure


void interrupt() {
    if (receiver.available()) {
        receiver.read(&data, sizeof(packet));
    }
}

void adjustServos(int x){
    // Takes in the x position of the joystick
    // Sends a signal to both aileron servos of a desired angles to rotate to
    leftAileronVal = map(x, 0, 255, 0, 180);
    rightAileronVal = 180 - leftAileronVal;
    leftAileron.write(leftAileronVal);
    rightAileron.write(rightAileronVal);
}

void adjustPropeller(int y){
    // Takes in the y position of the joystick
    // Sends a signal to the propeller motor to increase or decrease speed
    propellerVal = constrain(y, 80, 255);
    propellerVal = map(propellerVal, 80, 255, 1500, 2000);
    propeller.writeMicroseconds(propellerVal);
}

void onDisconnect(){
    // Returns servos and motors to their default position
    data.xpos = 127;
    data.ypos = 80;
    data.stick = 0;
    adjustServos(data.xpos);
    adjustPropeller(data.ypos);
}


void setup() {
    // Executed once when the arduino is first turned on
    Serial.begin(9600);                     // Baudrate of 9600
    leftAileron.attach(ServoL);
    rightAileron.attach(ServoR);
    propeller.attach(Prop);
    receiver.setAutoAck(1, false);          // Disable auto acknowledgement response when receiving a packet
    receiver.setDataRate(RF24_250KBPS);     // Minimum data rate
    receiver.setPALevel(RF24_PA_MAX);       // Maximum power amplification / may cause interference with interrupts
    receiver.maskIRQ(1, 1, 0);              // Masks all interrupts apart from when data is received
    receiver.openReadingPipe(1, address);   // Configure the radio as a receiver on pipe 1
    receiver.startListening();
    attachInterrupt(1, interrupt, FALLING); // When data is received trigger an interrupt and call the function interrupt
    currentTime = millis();
    lastTime = currentTime;
    onDisconnect();
}

void loop() {
    // Iterated continuousuly while the Arduino is powered
    currentTime = millis();
    if (receiver.available()) {             // If there is a connection to the receiver
        lastTime = currentTime;
        if (data.stick && lastToggle - currentTime < 1000) {   // If the stick has been pressed down more than a second before it was last pressed then disble controller inputs
            ontoggle = !ontoggle;
            lastToggle = currentTime;
        }
        if (ontoggle == 1) {    // If controller inputs are enabled then chaange the servos and motors to allign with the values received
            adjustServos(data.xpos);
            adjustPropeller(data.ypos);
        }
    }
    else if (currentTime - lastTime > 2000) {   // If no data has been received for 1 second 
        onDisconnect();
    }
}