# midiRouter
#### An Open-Source Midi Router


#### This projects aims to make MIDI easier to deal with, more accessible, and more affordable for musicians. 



###### Hardware To-Do:
- Produce and upload KiCad Schematics
- Add screen, encoder, and two buttons for menu system.
- Add expansion bus (SPI?) if hardware speed allows (CV out, etc)
- Design PCB (either rack mount or desktop module) 

###### Software To-Do:
- Unify how the code handles MIDI messages
- Add menu system
- Add MIDI filtration options to menu (clock source/ignore, paired TX/RX DIN ignoring, etc)
- Consider Pi Pico port


Currently a prototype has been built and used on a regular basis without issues. To prevent clock issues (more than one clock being broadcast), only one of the DIN ports listens for clock messages, the others will discard them (but all USB sources listen for clock). Please keep in mind, musical devices tend to be expensive, and improper wiring or other technical issues associated with home-made hardware can damage them. There is no warantee with this open source system. It may be unwise to risk damaging valuable instruments, especially if they can not be replaced.

For anyone wanting to make their own midi router, an overview of the prototype is below with more to come:


## Prototype Build:

![front image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/front.jpg)
![rear image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/rear.jpg)
![diagram image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/diagram.jpg)
![inside image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/inside.jpg)
![pcb image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/pcb.jpg)
![pcb rear image](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/pcb_rear.jpg)
![leds](https://github.com/FeverDreamEmulator/midiRouter/blob/main/images/leds.jpg)
