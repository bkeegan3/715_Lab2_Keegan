#include "DHT.h"
#include <TimeInterrupt.h>
#include <avr/io.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
#define temp_time 2
#define ledPin 9
#define ledPinY 10

uint8_t tim1Cnt=0;
uint8_t read_tmp_flg=0;
float total_time =0;
DHT dht(DHTPIN, DHTTYPE);

float temp_ary[128];
uint8_t temp_indx=0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  tim1Init();
  pinMode(ledPin , OUTPUT);
  pinMode( ledPinY, OUTPUT);

  sei(); // enable interrupts

}

void loop() {
  // put your main code here, to run repeatedly

  // 8.2 seconds have past read temp
  if(read_tmp_flg==1){
    read_tmp_flg=0;
    read_DHT();
  }
 
  temp_serial_tx();

}

//----------------------------------------------------------
//----------------DHT22-------------------------------------

void read_DHT (){

  float temp = dht.readTemperature(true); // read as °F with true

  if (isnan(temp)){
    // Serial.println("Missed DHT measurement");
    return;
  }

  temp_ary[ temp_indx ] = temp;
  
  if(temp_indx==127){
    temp_indx=127;
  } else {
    temp_indx++;
  }
  digitalWrite( ledPin, !digitalRead(ledPin) );
}

//----------------------------------------------------------
//----------------Serial Transfer---------------------------
void temp_serial_tx(){
  String data_cmd;
  if(Serial.available()==0){ 
    return;
    }
  while( Serial.available()==0 ){ // Wait for str input 
    }
  Serial.println("type data to read data");
  data_cmd = Serial.readStringUntil("\n");
  data_cmd.trim(); // readString adds garbage to the end of the string
  if (data_cmd!="data")
  {
    return;
  }


  cli();            // disable interrupts while txing data
  digitalWrite( ledPinY, !digitalRead( ledPinY) );
  
  for( int i=0 ; i<temp_indx ; i++){
    Serial.print(4.194*i); // (seconds) 65535 ticks / 15.625KHz
    Serial.print(", ");
    Serial.println( temp_ary[i], 2 );
  }
  Serial.end();
}

//----------------------------------------------------------
//----------------Timer 1-----------------------------------
void tim1Init(){
  TCCR1A = 0; // Reset timer1, configure for normal mode
		
  // clock_prescale=clk/1024			
  TCCR1B |= (1<<CS12) |(1<<CS10);
  TCCR1B &= ~(1<<CS11);

  TIMSK1 = (TOIE1|1); 	// Set timer1 for overflow interrupt enable 
}

ISR(TIMER1_OVF_vect){
  tim1Cnt++;

  if ( tim1Cnt >= temp_time ){
    read_tmp_flg=1;
    tim1Cnt=0;
  }
}




