# midiRouter
#### An Open-Source Midi Router


#### This project, originally published under my FeverDreamEmulator pseudonym, aims to make midi easier to deal with, more accessible, and more affordable for musicians. 

![front image](./images/front.jpg)

https://youtu.be/4NIHAnnQ0Rs

Thank you for stopping in and taking a look at the project! Please let me know if you make a router, it would be really neat to see someone benefitting from this. If anyone would like to collaborate, please feel encouraged. The hardware side of this project is much easier for me than the software, so working with this hodgepodge of midi objects and libraries to get all the ports working has not been the easiest, and I could use some help optimizing it. As far as I can tell it currently duplicates any type of midi message it receives, and then distributes it again properly, except in some USB hosting cases.

It would be really good to see this project turn into a more robust piece of open source DIY music equipment with community support! I've seen the potential for Teensys powering midi routers, and the sky is the limit.

To program, install [Arduino IDE](https://www.arduino.cc/en/software) and [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) addon in order to flash Teensy over USB.

## Response Times:

The unit takes approximately 0.05 milliseconds (50us) between receiving the end of a DIN MIDI message to forwarding that message out of all the output DIN ports. This was measured using PulseView, monitoring a logic analyzer connected to the input and 7 of the outputs.

![pulseview response](./images/pulseview_midi_test_view_2.png)
![pulseview response](./images/pulseview_midi_test_view_1.png)


## Construction Notes

Currently a prototype has been built and used on a regular basis without issues, usually just using DIN jacks and wall power or a usb power bank. I normally only have a couple devices hooked up, but I sometimes have 8 or more devices all talking together and working at-speed, and with Ableton continuous automation being sent as well. To prevent having to change clock options on individual instruments and also prevent more than one Midi Clock being broadcast at a time, only one of the DIN ports listens for clock messages (all USB sources listen for clock), the others will discard them. This can easily be changed to suit user preferences with an edit over USB with Arduino IDE. 

I use my midiRouter with all of my synthesizers, but I test all of my electrical connections before using them and I keep in mind that musical devices tend to be expensive, and improper wiring or other technical issues associated with home-made hardware can damage them. There is no warranty with this open source system. It may be unwise to risk damaging valuable instruments, especially if they can not be replaced.

For anyone wanting to make their own midi router, schematics and images of the prototype are below. Some things to note: 

- The kicad files use the [Teensy libraries from XenGi](https://github.com/XenGi/teensy_library)
- A cable extension is used to forward the Teensy's main USB to a [USB Type-B Female jack](https://www.adafruit.com/product/4212)
- Half of a USB cable extension is soldered to the Teensy's host pads to forward the Teensy's host USB to a [USB Type-A Female jack](https://www.adafruit.com/product/4214)
- DIN midi out is 3.3v, not 5v (no issues reported yet, transistors may be used for 5v instead if necessary)
- Unless a powered USB hub is used, the USB host port uses the USB client 5v and is limited by its available amperage

## Schematics:

![sheet 1](./schematics/midiRouter_schematic_1.jpg)
![sheet 2](./schematics/midiRouter_schematic_2.jpg)
![sheet 3](./schematics/midiRouter_schematic_3.jpg)


## Prototype Build:

![rear image](./images/rear.jpg)
![diagram image](./images/diagram.jpg)
![inside image](./images/inside.jpg)
![pcb image](./images/pcb.jpg)
![pcb rear image](./images/pcb_rear.jpg)
![leds](./images/leds.jpg)



###### Hardware To-Do:
- Produce and upload KiCad Schematics and BOM **[*In Progress*]** 
- Add screen, rotary encoder, and two buttons (3 with rotary click) for menu system
- Add expansion bus (SPI?) if hardware speed allows (CV out, etc)
- Design PCB (either rack mount or desktop module) to make sharing the build easier

###### Software To-Do:
- Optimize code (and also get midi objects looping instead of if() stacking)
- Get Korg instruments working over usb (shows up as two midi devices, usb host picks wrong one when connecting minilogue or monologue)
- Add menu system
- Add midi filtration options to menu (clock source/ignore, paired TX/RX DIN ignoring, etc)
- Port to Pi Pico

This PJRC page provides additional information on Teensy MIDI capabilities and basic circuits for DIN midi: https://www.pjrc.com/teensy/td_libs_MIDI.html The [/src/midiRouter.ino](./src/midiRouter.ino) file uses both MIDI.h and [USBHost_t36.h](https://github.com/PaulStoffregen/USBHost_t36) to process USB and DIN midi and also have them cross-interact. 
