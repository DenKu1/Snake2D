#include "Adafruit_NeoPixel.h"  // Подключаем библиотеку Adafruit NeoPixel. 
#define LED_COUNT 120  // Указываем, какое количество пикселей у нашей ленты. 
#define LED_PIN 6 // Указываем, к какому порту подключен вход ленты DIN.
#define LED_BRIGHTNESS 200
//константы
const int buttonPin1 = 2; 
const int buttonPin2 = 4;
const int buttonPin3 = 8;

const int ledPin1 =  12;
const int ledPin2 =  13;

unsigned long timing;

unsigned long exsistTrap;// Переменная для хранения точки отсчета

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Создаем переменную strip для управления нашей лентой. 
//Моя змейка
int zipperHead = 1;
int zipperLength = 3;
int zipperDirection = 0;
int pixelNum = 0;
//цвета
int color = 0;
uint32_t currentColor = strip.Color(255, 0, 255); // текущий цвет змейки
uint32_t zipperColors[5] = {strip.Color(24, 97, 22), strip.Color(255, 0, 25), strip.Color(55, 0, 255), strip.Color(255, 100, 255), strip.Color(55, 0, 55)}; // инициализация одномерного массива
//Препядствие
int trapPosition = random(LED_COUNT);
uint32_t trapColor = strip.Color(255, 81, 0); //цвет преграды
bool trapIsVisible = true;
//Еда
int foodPosition = 0;
void setup()
{
   Serial.begin(9600); //можно отключить отвечает за вывод

   pinMode(ledPin1, OUTPUT);  
   pinMode(ledPin2, OUTPUT);
     
   pinMode(buttonPin1, INPUT);  
   pinMode(buttonPin2, INPUT);   
   pinMode(buttonPin3, INPUT);
    
   strip.begin(); // Функция begin() настроит порт Arduino и выставит значения по умолчанию. 
   strip.setBrightness(LED_BRIGHTNESS);
   strip.setPixelColor(foodPosition, strip.Color(94, 0, 0)); //спаун еды

   trapSpawn();
   
   strip.show();  // Функция show() передаст команду на выключение всем пикселям. 
}


void zipper_draw()
{
    strip.clear();
    strip.setPixelColor(zipperHead, strip.Color(255, 255, 0));
  for (int i = 1; i < zipperLength; i++) 
  {
    
    if (zipperDirection == 0)
    {
      pixelNum = zipperHead - i;
    }
    else
    {
      pixelNum = zipperHead + i;
    }
      strip.setPixelColor(pixelNum, currentColor);   
     
    }

     strip.setPixelColor(foodPosition, strip.Color(94, 0, 0)); // Отвечает за появление еды
     trapSpawn();
     strip.show();
}
void trapSpawn()
{
     for (int i = 0; i < 3; i++) 
     {
        strip.setPixelColor(trapPosition + i, trapColor); //спаун ловушки
     }
}

void pulsar()
{
 
      if (millis() - timing > 400)
     { 
        timing = millis();    
       
        if (trapIsVisible == true)
        {  // и инвертируем состояние светодиода
           trapColor = strip.Color(0, 0, 0); 
           trapIsVisible = false;
           
        }
        else 
        {
           trapColor = strip.Color(255, 81, 0); 
           trapIsVisible = true;
        }
        for (int i = 0; i < 3; i++) 
        {
        strip.setPixelColor(trapPosition + i, trapColor); //спаун ловушки
        }
        strip.show();
    }


    if (millis() - exsistTrap > 3200)
     { 
        exsistTrap = millis();    
        trapColor = strip.Color(0, 0, 0); 
        trapSpawn();
        strip.show();
        trapPosition = random(LED_COUNT);
     }
}

void loop()
{
  pulsar(); //ловушка мигает
    
  if (digitalRead(buttonPin1) == HIGH && zipperHead < LED_COUNT-1)
  {
    
    zipperHead++;
    zipperDirection = 0; 
    zipper_draw();
    delay(20);
  } 
  else if (digitalRead(buttonPin2) == HIGH && zipperHead > 0)
  {
    zipperHead--;   
    zipperDirection = 1;
    zipper_draw();
    delay(20);

  } 
  if (digitalRead(buttonPin3) == HIGH)
  {
    
    digitalWrite(ledPin2, HIGH); 
    currentColor = zipperColors[color];
    if (color == 4)
    {
      color = 0;  
    }
    else
    {
      color++;
    }
    delay(500);
    
  } 
  else
  {
    digitalWrite(ledPin2, LOW); 
  }
  if (foodPosition == zipperHead) //проверка сьела ли змея еду
  {
    zipperLength++;
    strip.setPixelColor(foodPosition, strip.Color(0, 0, 0));
    foodPosition = random(LED_COUNT);
    strip.show();
  }
  if (zipperHead == trapPosition || zipperHead == trapPosition)
  {
      zipperLength = 2;  
      trapPosition = random(LED_COUNT);
  }
}
