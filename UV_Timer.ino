/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers
Pick one up today in the adafruit shop!
------> http://www.adafruit.com/category/63_98
This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 9
#define alrt_pin 4
#define uvlp_pin 8
#define push_pin 3

Adafruit_SSD1306 display(OLED_RESET);
float timeleftPCB = 150;
float timeleftMSK = 900;
bool mode = false; //mode 0 = PCB, mode 1 = mask

void showval(float val)
{
    display.clearDisplay();
    display.setCursor(5, 20);
    display.print(val, 1);
    display.display();
}

void beep(int times)
{
  for (int i=0;times>i;i++)
  {
          digitalWrite(alrt_pin, HIGH);
          delay(150);
          digitalWrite(alrt_pin, LOW);
          delay(300);

  }
}

void modeSelect()
{
  display.clearDisplay();
  
  if (mode) 
    { 
      display.setCursor(30, 20);
      display.print("PCB"); 
    }
  else 
    {
      display.setCursor(15, 20);
      display.print("MASK");
    }
  display.display();

}

void setup()
{
  pinMode(uvlp_pin, OUTPUT);
  pinMode(alrt_pin, OUTPUT);
  pinMode(push_pin, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(4);
  digitalWrite(uvlp_pin, HIGH); 
  delay(100);
  digitalWrite(uvlp_pin, LOW); 
  beep(1);
  modeSelect();

}


//Mostrar  el nro de la cuenta regresiva
// si presionan el boton.. iniciar
// si presionan una vez mas. detener
// si long press detener
float tcounter =  0;
int pcounter =  0;
int longCount = 0;
bool flagcount = false;
bool longpress = false;

void loop()
{
    if(!digitalRead(push_pin)) 
      {
        delay(10);
        longpress=false, longCount=0;
        while(!digitalRead(push_pin))
        {
          delay(100);
          longCount++;
          if (longCount>=10) {longpress=true; break;}
        }
        if(longpress)
          {
            delay(100);
            tcounter = !mode*timeleftMSK + mode*timeleftPCB;
            flagcount=false;
            while(true)
            {
              showval(tcounter);
              delay(9);
              if(!digitalRead(push_pin)) 
                {
                  delay(10);
                  flagcount=!flagcount;
                  longpress=false, longCount=0;
                  while(!digitalRead(push_pin))
                  {
                    delay(100);
                    longCount++;
                    if (longCount>=10) {longpress=true; break;}
                  }
                  if(!digitalRead(push_pin))
                    {
                      delay(100);
                      digitalWrite(uvlp_pin, LOW);
                      break;
                      }
                  beep(1);
                }
              

              if (flagcount)
              {
                if (tcounter>0.05) 
                  { 
                    tcounter = tcounter - 0.05; 
                    digitalWrite(uvlp_pin, HIGH);
                  }
                else 
                  { 
                    flagcount=false; beep(3); 
                    digitalWrite(uvlp_pin, LOW);
                    mode=!mode; break;
                  }
              }
              else
                digitalWrite(uvlp_pin, LOW);
            }
              
          } 
        mode=!mode;
        modeSelect();

        beep(1);
      }
    

}



