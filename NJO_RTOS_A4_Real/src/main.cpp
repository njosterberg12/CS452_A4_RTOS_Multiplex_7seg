#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "SevSegNum.h"
#include "semphr.h"
#include "queue.h"

#define LED_BUILTIN 13

#define SevenSegCC1 44
#define SevenSegCC2 46

#define SevenSegA 4
#define SevenSegB 5
#define SevenSegC 6
#define SevenSegD 7
#define SevenSegE 8
#define SevenSegF 9
#define SevenSegG 10
#define SevenSegDP 11

QueueHandle_t leftDigQueue = 0;
QueueHandle_t rightDigQueue = 0;
SemaphoreHandle_t blinkSem = 0;


void vDisplayLeft(void *pvParameters);
void vDisplayRight(void *pvParameters);

void vCounter1(void *pvParameters);

void vBlink1(void *pvParameters );

void setup() {

  pinMode(SevenSegA, OUTPUT);
  pinMode(SevenSegB, OUTPUT);
  pinMode(SevenSegC, OUTPUT);
  pinMode(SevenSegD, OUTPUT);
  pinMode(SevenSegE, OUTPUT);
  pinMode(SevenSegF, OUTPUT);
  pinMode(SevenSegG, OUTPUT);
  pinMode(SevenSegDP, OUTPUT);

  pinMode(SevenSegCC1, OUTPUT);
  pinMode(SevenSegCC2, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);

  while(!Serial)
  {
    ;
  }

  leftDigQueue = xQueueCreate(1, sizeof (int));
  rightDigQueue = xQueueCreate(1, sizeof (int));

  xTaskCreate(vDisplayLeft, "LDig", 128, NULL, 1, NULL);
  xTaskCreate(vDisplayRight, "RDig", 128, NULL, 3, NULL);
  xTaskCreate(vCounter1, "Counter", 128, NULL, 1, NULL);

  xTaskCreate(vBlink1, "Blink LED", 64, NULL, 0, NULL);

  vTaskStartScheduler();

}

void loop() {}

void vDisplayLeft(void *pvParameters)
{
  (void) pvParameters;
  int i;
  for(;;)
  {
  if(!xQueueReceive(leftDigQueue, &i, 16 / portTICK_PERIOD_MS))
  {
  }
  digitalWrite(SevenSegCC2, HIGH);
    switch(i) // switches received queue data to display whatever left dig needed
    {
      case 0:
        printNum0on();
        printNum0off();
        break;
      case 1:
        printNum1on();
        printNum1off();
        break;
      case 2:
        printNum2on();
        printNum2off();
        break;
      case 3:
        printNum3on();
        printNum3off();
        break;
      case 4:
        printNum4on();
        printNum4off();
        break;
    }
  digitalWrite(SevenSegCC2, LOW); 
  }
}

void vDisplayRight(void *pvParameters)
{
  (void) pvParameters;

  int i;
  for(;;)
  {
  if(!xQueueReceive(rightDigQueue, &i, 16 / portTICK_PERIOD_MS))
  {
  }
  digitalWrite(SevenSegCC1, HIGH);
    switch(i) // switches received queue data to display whatever right dig needed
    {
      case 0:
        printNum0on();
        printNum0off();
        break;
      case 1:
        printNum1on();
        printNum1off();
        break;
      case 2:
        printNum2on();
        printNum2off();
        break;
      case 3:
        printNum3on();
        printNum3off();
        break;
      case 4:
        printNum4on();
        printNum4off();
        break;
      case 5:
        printNum5on();
        printNum5off();
        break;
      case 6:
        printNum6on();
        printNum6off();
        break;
      case 7:
        printNum7on();
        printNum7off();
        break;
      case 8:
        printNum8on();
        printNum8off();
        break;
      case 9:
        printNum9on();
        printNum9off();
        break;
    }
  digitalWrite(SevenSegCC1, LOW);
  }
}

/*********************************************************
 * void vBlink1(void *pvParameters)
 * 
 * This function blinks the D13 LED every half second in 
 *  conjuction with the counter Function
 * *******************************************************/
void vBlink1(void *pvParameters)
{
  (void) pvParameters;

  pinMode(13, OUTPUT);
  for(;;)
  {
    digitalWrite(13, HIGH);
    vTaskDelay(490 / portTICK_PERIOD_MS);
    digitalWrite(13, LOW);
    vTaskDelay(490/ portTICK_PERIOD_MS);
  }
}

/***********************************************************
 * void vCounter1(void *pvParameters)
 * 
 * This function counts from 0 to 42 and back down to 0.
 * It is called immediately after the 7seg displays change
 * *********************************************************/

void vCounter1(void *pvParameters) // think about it as main
{
  (void) pvParameters;

  if(leftDigQueue == 0)
  {
  }

  int i, x, y;

  for(;;)
  {
    for(i = 0; i < 43; i++)
    {
      //Serial.println(i);
      vTaskDelay(10 / portTICK_PERIOD_MS);
      x = i / 10; // for determining right digit
      if(!xQueueSend(leftDigQueue, &x, portMAX_DELAY))
      {
      }
      vTaskDelay(20 / portTICK_PERIOD_MS);
      y = i % 10; // for determining left digit
      if(!xQueueSend(rightDigQueue, &y, portMAX_DELAY))
      {
      }
      vTaskDelay(470 / portTICK_PERIOD_MS);
    }
    for(i = 41; i > 0; i--)
    {
      //Serial.println(i);
      x = i / 10; // for determining right digit
      vTaskDelay(10 / portTICK_PERIOD_MS);      
      if(!xQueueSend(leftDigQueue, &x, portMAX_DELAY))
      {
      }
      vTaskDelay(20 / portTICK_PERIOD_MS);
      y = i % 10; // for determining left digit
      if(!xQueueSend(rightDigQueue, &y, portMAX_DELAY))
      {
      }
      vTaskDelay(470 / portTICK_PERIOD_MS);
    }
  }
}
