# 715_Lab2_Keegan

## Requirements
Develop a C or C++ application which executes on an Arduino that captures temperature data from a temperature sensor at a periodic rate. The temperature collected from the sensor must be recorded in Fahrenheit. The period to collect data will be roughly at a rate of 1 sample every 10 seconds. To test the device it must collect 10 minutes worth of data. The first five minutes the arduino and temperature sensor must be in a freezer. The next five minutes will be collected in a room temperature environment. The data needs to be trasmitted to a a host PC through USB, RS232, SPI, i2c, etc. Once the data is captured, plot temperature versus time.

## Design
### Hardware

### Breadboard

### reset enable


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

## Video
