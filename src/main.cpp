#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <Adafruit_NeoPixel.h>
#include <Rotary.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

//MATRIZ LED
#define DIN_WS2812  PE2
// How many NeoPixels are attached?
#define NUMPIXELS 10 // Popular NeoPixel ring size

#define FILAS 10
#define COLUMNAS 10
int matriz[FILAS][COLUMNAS];

Adafruit_NeoPixel pixels(NUMPIXELS, DIN_WS2812, NEO_GRB + NEO_KHZ800);

//LEDS EN PLACA
#define LED_1 PF10
#define LED_2 PF9

//ENCODER
#define DT PE0
#define CLK PE1
//#define SWITCH PE3

// Counter that will be incremented or decremented by rotation.
volatile int counter = 0;

  
// Rotary encoder is wired with the common to ground and the two
// outputs to DT and CLK.
Rotary rotary = Rotary(CLK, DT);

//FREERTOS

/*
 * TAREA MANEJO DE ENCODER POR POOLEO
 */
static void task_encoder(void* arg) {
  UNUSED(arg);

  // Counter that will be incremented or decremented by rotation.
  int posicion = 0;

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  digitalWrite(LED_1, HIGH); //EL LED_1 SE APAGA CON UN HIGH
  digitalWrite(LED_2, HIGH);  //EL LED_2 SE APAGA CON UN HIGH

  //LLENO MATRIZ
  for(int i=0; i < FILAS; i++){
    for(int j=0; j < COLUMNAS; j++){
      matriz[i][j] = (i*10)+j;
    }
  }

  pixels.begin();

  pixels.clear();
  pixels.setPixelColor(matriz[0][posicion], pixels.Color(75,50,0));
  pixels.show();

  while (1) {
    if(counter != posicion){
      posicion = counter;
      pixels.clear();
      pixels.setPixelColor(matriz[0][posicion], pixels.Color(75,50,0));
      pixels.show();
    }
    // Sleep for 200 milliseconds.
    vTaskDelay((10L * configTICK_RATE_HZ) / 1000L);
  }
}

void rotate(void);

void setup()
{
  attachInterrupt(DT, rotate, CHANGE);
  attachInterrupt(CLK, rotate, CHANGE);
  // create task at priority one
  xTaskCreate(task_encoder, (const char *)"task_encoder", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
  // start scheduler
  vTaskStartScheduler();

  while (1);  
}

void loop(){
}

// rotate is called anytime the rotary inputs change state.
void rotate() {
  unsigned char result = rotary.process();
  if (result == DIR_CW) {
    counter++;
  } else if (result == DIR_CCW) {
    counter--;
  }
  counter = MIN(9,MAX(0,counter)); 
}