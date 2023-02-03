#include "DHT.h"
#include <TimeInterrupt.h>
#include <avr/io.h>

#define DHTPIN 2 
#define DHTTYPE DHT22

uint8_t tim1Cnt=0;
uint8_t read_tmp_flg=0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  tim1Init();

  sei(); // enable interrupts

}

void loop() {
  // put your main code here, to run repeatedly
  if(read_tmp_flg==1){
    read_tmp_flg=0;

    read_DHT();
  }

}

void read_DHT (){

  float temp = dht.readTemperature(true); // read as °F with true

  if (isnan(temp)){
    Serial.println("Missed DHT measurement");
    return;
  }

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
  tim1Cnt++;
  if (tim1Cnt==2){
    read_tmp_flg=1;
    tim1Cnt=0;
  }

	
}


