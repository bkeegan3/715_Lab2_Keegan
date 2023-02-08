# 715_Lab2_Keegan

## Requirements
Develop a C or C++ application which executes on an Arduino that captures temperature data from a temperature sensor at a periodic rate. The temperature collected from the sensor must be recorded in Fahrenheit. The period to collect data will be roughly at a rate of 1 sample every 10 seconds. To test the device it must collect 10 minutes worth of data. The first five minutes the arduino and temperature sensor must be in a freezer. The next five minutes will be collected in a room temperature environment. The data needs to be trasmitted to a a host PC through USB, RS232, SPI, i2c, etc. Once the data is captured, plot temperature versus time.

## Design
### Hardware
For the temperature sensor I used the DHT22. Once provided 5V and gnd the DHT22 provides its digital temperature output on pin 2 of the device. Instead of using a 10 foot serial cable to transmit data I opted to operate the arduino via battery power for mobile operation. Once the 10 minutes are complete I would connect up to a serial port to transmit the data. 
![]() 
### Breadboard

### Reset enable
A good amount of troubleshooting was spent on connecting up the serial port of the arduino to the PC. After research online I found this [post](https://playground.arduino.cc/Main/DisablingAutoResetOnSerialConnection/) that the DTR signal resets the arduino. The methods provided in that post were to add a resistor between reset and 5V or and a capacitor between reset and gnd; neither of those solutions worked for me. The more invasive method mentioned in that post is to cut the trace between the two pads highlighted by the white RESET_EN overlay !()[]. This does affect how code is uploaded to the arduino now that the device will not auto reset when a usb is connected. The easiest way to work around this is to jumper the two pads with some jumper wire.

### Software
For software I followed the lab instructions and used a Round Robbin with Interrupts design. To tackle getting periodic measurements from the DHT22 I used timer1 on the ATmega328p which is a 16bit timer. To

### Timer1
The 16bit timer is not able to count up to 10 seconds. The highest possible value, using a prescaler = 1024 to divide the clock, is

   16MHz/1024  <br>
  -------------- = 0.238 Hz/tick = 4.194 seconds/overflow  <br>
   (2^16 - 1) ticks  

This could be used to achieve a period of 4.2 seconds, 8.4 seconds, 12.6 seconds, and etc. to get approxiametly to 10 seconds I will count each time the overflow flag is set in the intterupt service routine. To get closer to the 10 second interrupt rate I used the output compare registers. The ATmega328P datasheet states that " ...The 16-bit comparator continuously compares TCNT1 with the output compare register (OCR1x). If TCNT equals OCR1x the comparator signals a match. A match will set the output compare flag (OCF1x) at the next timer clock cycle. If enabled (OCIE1x = 1), the output compare flag generates an output compare interrupt." Using the OCR1A register I will configure timer1 to interrupt when the output compare A register and time count register match. The time needed is 10 - 8.388 = 1.612 seconds. The value that needs to be input into the OCF1A register is  

   16MHz * 1.612 seconds  
  ---------------------- = 25,187 ticks
<br>   1024    

## Temperature Plot


## Video
