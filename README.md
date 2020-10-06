# SpeakJammer
[Speakjet](https://www.sparkfun.com/products/9578) based synthesizer module. 

- 4 separate programmable sentences, or sound effects
- 4x Trigger input 
- Volume CV in
- Speed CV in
- Pitch CV in 
- Bend CV in 
- Eurorack level output

## Hardware
Utilizes the [Seeeduino Mega](https://www.kiwi-electronics.nl/seeeduino-mega?gclid=CjwKCAjwq_D7BRADEiwAVMDdHktkGHXA0BFi8trz0PDWnKK1Qx9IWdeiD50j2ThxfVhXpcyWRFPS6hoCgA4QAvD_BwE),
[VoiceBox Shield](https://www.sparkfun.com/products/retired/9799) and [128x64 liquid crystal display](https://wiki.dfrobot.com/3-wire_Serial_LCD_Module__Arduino_Compatible___SKU_DFR0091_)

### Schematic
TODO
### Bom
TODO

## Software
- [Arduino sketch](speakjammer.ino)
- [lcd sketch](lcd.ino)
- [speakjet sketch](speakjet.ino)

##Used libraries
- [u8g2](https://github.com/olikraus/u8g2)
- [ClickEncoder](https://github.com/0xPIT/encoder)
- [TimerOne](https://github.com/PaulStoffregen/TimerOne)