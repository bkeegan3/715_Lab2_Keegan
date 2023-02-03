#include "DHT.h"
#include <TimeInterrupt.h>
#include <avr/io.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
#define Temp_delay 10000
#define ledPin 9

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  pinMode(ledPin, OUTPUT);
  tim1Init();

  sei(); // enable interrupts

}

void loop() {
  // put your main code here, to run repeatedly

}

void read_DHT (){

  float temp = dht.readTemperature(true); // read as °F with true

  if (isnan(temp)){
    Serial.println("Missed DHT measurement");
    return;
  }

  // digitalWrite(ledPin, !digitalRead(ledPin)); // toggle the led pin

  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.println(F("°F"));
 

}

void tim1Init(){
  TCCR1A = 0; // Reset timer1, configure for normal mode
		
  // clock_prescale=clk/1024			
  TCCR1B |= (1<<CS12) |(1<<CS10);
  TCCR1B &= ~(1<<CS11);

  TIMSK1 = (TOIE1|1); 	// Set timer1 for overflow interrupt enable
  
}

ISR(TIMER1_OVF_vect){
	digitalWrite(ledPin, !digitalRead(ledPin));
}


