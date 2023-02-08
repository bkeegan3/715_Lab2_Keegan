#include "DHT.h"
#include <avr/io.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
#define temp_time 3
#define ledPin 9
#define ledPinY 10

uint8_t tim1Cnt=0;
uint8_t tim1tick = 25178; // 2 ovrflw + 25178 ticks = 10 seconds
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

  // 10 seconds have past read temp
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
    Serial.print(10*i); // (seconds) 65535 ticks / 15.625KHz
    Serial.print(", ");
    Serial.println( temp_ary[i], 2 );
  }
  Serial.end();
}

//----------------------------------------------------------
//----------------Timer 1-----------------------------------
void tim1Init(){
  TCCR1A = 0; // Reset timer1, configure for normal mode
		
  // prescaler = 1024			
  TCCR1B |= (1<<CS12) |(1<<CS10);
  TCCR1B &= ~(1<<CS11);

  //  2 ovrflw + 25178 ticks = 10 seconds
  OCR1A = tim1tick;

  TIMSK1 = (1<<OCIE1A); 	// Set timer1 for  interrupt enable at OC1A
}

ISR(TIMER1_COMPA_vect ){
  tim1Cnt++;

  if ( tim1Cnt >= temp_time ){
    read_tmp_flg=1;
    tim1Cnt=0;
  }
}




