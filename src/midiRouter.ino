/* 

  Open Soure Midi Router
  https://github.com/FeverDreamEmulator/midiRouter

  This file is used with Teensyduino to program a Teensy uC into a midi router

  UNABLE TO FIND usbMIDI?
  You must select a midi type in the usb options for the chip before compiling or it will fail: UNABLE TO FIND usbMIDI 
  
*/

#include <Arduino.h>
#include <MIDI.h>
#include <USBHost_t36.h> // access to USB MIDI devices (plugged into 2nd USB port)
//#include <SoftwareSerial.h>

/*
//pins for software bit-banged MIDI DIN ports - significantly slower than hardware ports
SoftwareSerial softSerial1(2, 3); //rx, tx
SoftwareSerial softSerial2(4, 5); //rx, tx
SoftwareSerial softSerial3(6, 9); //rx, tx
*/


// LED Variables
#define YELLOW_PIN 2
#define GREEN_PIN 3
long unsigned clockMillis = 0;  // variable for led timer
long unsigned midiMillis = 0;   // variable for led timer
bool clockLamp = false;         // prevent unneccesary writing to led pins
bool midiLamp = false;          // prevent unneccesary writing to led pins


// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI4);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI5);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI6);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial7, MIDI7);


// Create the ports for USB devices plugged into Teensy's 2nd USB (host）port 
// (port expandable using physical usb hubs, however power concerns should be considered)
//#define USBHOST_QTY 10
USBHost myusb;
USBHub hub1(myusb);
//USBHub hub2(myusb);
//USBHub hub3(myusb);
//USBHub hub4(myusb);
MIDIDevice midi01(myusb);
MIDIDevice midi02(myusb);
MIDIDevice midi03(myusb);
MIDIDevice midi04(myusb);
MIDIDevice midi05(myusb);
MIDIDevice midi06(myusb);
MIDIDevice midi07(myusb);
MIDIDevice midi08(myusb);
MIDIDevice midi09(myusb);
MIDIDevice midi10(myusb);

MIDIDevice * midilist[10] = { 
  &midi01, &midi02, &midi03, &midi04, &midi05, &midi06, &midi07, &midi08, &midi09, &midi10
};

int USBrcvd = -1; //track which USB port recevied a message so as not to resend it back out to same port



void setup() {
  
  Serial.begin(115200);  //for serial connection over usb to PC
  
  //status LEDs
  pinMode(YELLOW_PIN, OUTPUT); // yellow LED
  digitalWrite(YELLOW_PIN, HIGH);
  pinMode(GREEN_PIN, OUTPUT);  // green LED
  digitalWrite(GREEN_PIN, HIGH);
  
  //initialize DIN connectors
  MIDI1.begin(MIDI_CHANNEL_OMNI);
  MIDI2.begin(MIDI_CHANNEL_OMNI);
  MIDI3.begin(MIDI_CHANNEL_OMNI);
  MIDI4.begin(MIDI_CHANNEL_OMNI);
  MIDI5.begin(MIDI_CHANNEL_OMNI);
  MIDI6.begin(MIDI_CHANNEL_OMNI);
  MIDI7.begin(MIDI_CHANNEL_OMNI);
  //initialize USB Host connections
  // Wait 1.5 seconds before turning on USB Host.  If connected USB devices
  // use too much power, Teensy at least completes USB enumeration, which
  // makes isolating the power issue easier.
  delay(1500);
  myusb.begin();
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  
  
}



void loop() {

  // re-init loop variables
  long unsigned currentMillis = millis();  // for timer watchdogs
  bool clockActivity = false;
  bool midiActivity = false;
  USBrcvd = -1;
  
  if (MIDI1.read()) {
    byte commandType = MIDI1.getType();
    //suppress receiving midi clock over DIN
    sendMIDI(commandType, MIDI1.getChannel(), MIDI1.getData1(), MIDI1.getData2(), MIDI1.getSysExArray());
    if (commandType == 0xF8) clockActivity = true;
    else midiActivity = true;
  }

  if (MIDI2.read()) {
    byte commandType = MIDI2.getType();
    //suppress receiving midi clock over DIN
    if (commandType != 0xF8) {
      sendMIDI(commandType, MIDI2.getChannel(), MIDI2.getData1(), MIDI2.getData2(), MIDI2.getSysExArray());
      midiActivity = true;
    }
  }

  if (MIDI3.read()) {
    byte commandType = MIDI3.getType();
    //suppress receiving midi clock over DIN
    if (commandType != 0xF8) {
      sendMIDI(commandType, MIDI3.getChannel(), MIDI3.getData1(), MIDI3.getData2(), MIDI3.getSysExArray());
      midiActivity = true;
    }
  }

  if (MIDI4.read()) {
    byte commandType = MIDI4.getType();
    //suppress receiving midi clock over DIN
    if (commandType != 0xF8) {
      sendMIDI(commandType, MIDI4.getChannel(), MIDI4.getData1(), MIDI4.getData2(), MIDI4.getSysExArray());
      midiActivity = true;
    }
  }

  if (MIDI5.read()) {
    byte commandType = MIDI5.getType();
    //suppress receiving midi clock over DIN
    if (commandType != 0xF8) {
      sendMIDI(commandType, MIDI5.getChannel(), MIDI5.getData1(), MIDI5.getData2(), MIDI5.getSysExArray());
      midiActivity = true;
    }
  }

  //only 5 DIN read ports
  /*
  if (MIDI6.read()) {
    sendMIDI(MIDI6.getType(), MIDI6.getChannel(), MIDI6.getData1(), MIDI6.getData2(), MIDI6.getSysExArray());
  }

  if (MIDI7.read()) {
    sendMIDI(MIDI7.getType(), MIDI7.getChannel(), MIDI7.getData1(), MIDI7.getData2(), MIDI7.getSysExArray());
  }
  */

  if (usbMIDI.read()) {
    byte commandType = usbMIDI.getType();
    USBrcvd = 0;
    sendMIDI(commandType, usbMIDI.getChannel(), usbMIDI.getData1(), usbMIDI.getData2(), usbMIDI.getSysExArray()); //usbMIDI.getCable());
    if (commandType == 0xF8) clockActivity = true;
    else midiActivity = true;
    
    
  }
  
  // Next read messages arriving from the (up to) 10 USB devices plugged into the USB Host port
  for (int port=0; port < 10; port++) {
    if (midilist[port]->read()) {
      uint8_t type =       midilist[port]->getType();
      uint8_t data1 =      midilist[port]->getData1();
      uint8_t data2 =      midilist[port]->getData2();
      uint8_t channel =    midilist[port]->getChannel();
      const uint8_t *sys = midilist[port]->getSysExArray();
      USBrcvd = port + 1;
      sendMIDI(type, channel, data1, data2, sys);
      if (type == 0xF8) clockActivity = true;
      else midiActivity = true;
    }
  }
  
  
  /*
  if (midi01.read()) {
    sendMIDI(midi01.getType(), midi01.getChannel(), midi01.getData1(), midi01.getData2(), midi01.getSysExArray());
  }

  if (midi02.read()) {
    sendMIDI(midi02.getType(), midi02.getChannel(), midi02.getData1(), midi02.getData2(), midi02.getSysExArray());
  }
  
  if (midi03.read()) {
    sendMIDI(midi03.getType(), midi03.getChannel(), midi03.getData1(), midi03.getData2(), midi03.getSysExArray());
  }
  
  if (midi04.read()) {
    sendMIDI(midi04.getType(), midi04.getChannel(), midi04.getData1(), midi04.getData2(), midi04.getSysExArray());
  }
  
  if (midi05.read()) {
    sendMIDI(midi05.getType(), midi05.getChannel(), midi05.getData1(), midi05.getData2(), midi05.getSysExArray());
  }
  
  if (midi06.read()) {
    sendMIDI(midi06.getType(), midi06.getChannel(), midi06.getData1(), midi06.getData2(), midi06.getSysExArray());
  }
  
  if (midi07.read()) {
    sendMIDI(midi07.getType(), midi07.getChannel(), midi07.getData1(), midi07.getData2(), midi07.getSysExArray());
  }
  
  if (midi08.read()) {
    sendMIDI(midi08.getType(), midi08.getChannel(), midi08.getData1(), midi08.getData2(), midi08.getSysExArray());
  }
  
  if (midi09.read()) {
    sendMIDI(midi09.getType(), midi09.getChannel(), midi09.getData1(), midi09.getData2(), midi09.getSysExArray());
  }
  
  if (midi10.read()) {
    sendMIDI(midi10.getType(), midi10.getChannel(), midi10.getData1(), midi10.getData2(), midi10.getSysExArray());
  }
  */
  
  
  if (clockActivity && !clockLamp) {
    digitalWrite(YELLOW_PIN, HIGH);
    clockLamp = true;
    clockMillis = currentMillis;
  } else if ((currentMillis > clockMillis + 25) && clockLamp) {
    digitalWrite(YELLOW_PIN, LOW);
    clockLamp = false;
  }
  if (midiActivity && !midiLamp) {
    digitalWrite(GREEN_PIN, HIGH);
    midiLamp = true;
    midiMillis = currentMillis;
  } else if ((currentMillis > midiMillis + 25) && midiLamp) {
    digitalWrite(GREEN_PIN, LOW);
    midiLamp = false;
  }
  
}


// When messages are received, this function sends the message to all the devices
void sendMIDI(byte type, byte channel, byte data1, byte data2, const uint8_t *sysexarray) {
  
  if (type != midi::SystemExclusive) {
    midi::MidiType mtype = (midi::MidiType)type;
    
    // Normal messages, simply give the data to the usbMIDI.send()
    MIDI1.send(mtype, data1, data2, channel);
    MIDI2.send(mtype, data1, data2, channel);
    MIDI3.send(mtype, data1, data2, channel);
    MIDI4.send(mtype, data1, data2, channel);
    MIDI5.send(mtype, data1, data2, channel);
    MIDI6.send(mtype, data1, data2, channel);
    MIDI7.send(mtype, data1, data2, channel);
    
    if (USBrcvd != 0) usbMIDI.send(type, data1, data2, channel, 0);
    
    if (USBrcvd != 1) midi01.send(type, data1, data2, channel);
    if (USBrcvd != 2) midi02.send(type, data1, data2, channel);
    if (USBrcvd != 3) midi03.send(type, data1, data2, channel);
    if (USBrcvd != 4) midi04.send(type, data1, data2, channel);
    if (USBrcvd != 5) midi05.send(type, data1, data2, channel);
    if (USBrcvd != 6) midi06.send(type, data1, data2, channel);
    if (USBrcvd != 7) midi07.send(type, data1, data2, channel);
    if (USBrcvd != 8) midi08.send(type, data1, data2, channel);
    if (USBrcvd != 9) midi09.send(type, data1, data2, channel);
    if (USBrcvd != 10) midi10.send(type, data1, data2, channel);
    
  } else {
    
    // SysEx messages are special.  The message length is given in data1 & data2
    unsigned int SysExLength = data1 + data2 * 256;
    MIDI1.sendSysEx(SysExLength, sysexarray, true);
    MIDI2.sendSysEx(SysExLength, sysexarray, true);
    MIDI3.sendSysEx(SysExLength, sysexarray, true);
    MIDI4.sendSysEx(SysExLength, sysexarray, true);
    MIDI5.sendSysEx(SysExLength, sysexarray, true);
    MIDI6.sendSysEx(SysExLength, sysexarray, true);
    MIDI7.sendSysEx(SysExLength, sysexarray, true);
    
    if (USBrcvd != 0) usbMIDI.sendSysEx(SysExLength, sysexarray, true, 0);
    
    if (USBrcvd != 1) midi01.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 2) midi02.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 3) midi03.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 4) midi04.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 5) midi05.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 6) midi06.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 7) midi07.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 8) midi08.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 9) midi09.sendSysEx(SysExLength, sysexarray, true);
    if (USBrcvd != 10) midi10.sendSysEx(SysExLength, sysexarray, true);
    
  }
  
 // usbMIDI.send_now(); //causes hangs if no device is plugged into USB host
 // usbMIDI.flush();
  
}
