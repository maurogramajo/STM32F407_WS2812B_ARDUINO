#include <Arduino.h>

/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#define LED_1 PF10
#define LED_2 PF9

//ENCODER
#define DT PE0
#define CLK PE1
//#define SWITCH 3

int anterior = 0;
volatile int posicion = 0;

void encoder(void);

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  pinMode(DT,INPUT);
  pinMode(CLK,INPUT);

  attachInterrupt(digitalPinToInterrupt(DT), encoder, LOW);

  digitalWrite(LED_1, HIGH); //EL LED_1 SE APAGA CON UN HIGH
  digitalWrite(LED_2, HIGH);  //EL LED_2 SE APAGA CON UN HIGH

}

void loop()
{
  if(posicion != anterior)
    anterior = posicion;

  switch(anterior){
    case 0:
      digitalWrite(LED_1, LOW);
      delay(1000);
      digitalWrite(LED_1, HIGH);
      delay(1000);
      break;
    case 1:
      digitalWrite(LED_2, LOW);
      delay(500);
      digitalWrite(LED_2, HIGH);
      delay(500);
      break;
    default:
      break;
  }
}

void encoder(){
  static unsigned long ultimainterr = 0;
  unsigned long tiempointerr = millis();
  
  if(tiempointerr - ultimainterr > 5){
    if(digitalRead(CLK) == HIGH)
        posicion--;
    else
        posicion++;

    if(posicion < 0){
      posicion = 0;
    }else if(posicion > 1){
      posicion = 1;
    }

    ultimainterr = tiempointerr;
  }
}