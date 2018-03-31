// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include "U8glib.h"
#include <Servo.h>


//pin defines
#define lcd_cs 10
#define dc     8
#define rst    7
#define encA 3 //Enconder output A
#define encB 4 //Enconder output B
#define encC 2 //Enconder button output
#define ESCpin 9  //Pin for ESC control
#define E_ESC 6 //ESC enable

#define TIMER2ON  TIMSK2=(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<OCIE2B)|(0<<OCIE2A)|(1<<TOIE2)
#define TIMER2OFF TIMSK2=(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<OCIE2B)|(0<<OCIE2A)|(0<<TOIE2)


///////////Functions//////////
//interruption functions
int pressed ();
int moved ();
ISR(TIMER2_OVF_vect);

//general functions
void manageLastPress();
bool EncoderDirection ();
void startRotor (int rpm, unsigned int _min); 
void pauseRotor();
void DrawMenu(int _menu);
void spin();

//initialization function
void initialize ();

/////Variable initialization/////

//Time control
unsigned int counter = 0;

//Velocity control
int rpm = 2000;
unsigned int _min = 1;
unsigned int sec = 0;

//ESC
Servo ESC; 

//Screen
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

//encoder
int aState;
int aLastState;
int pulsado;
int movido = 100;
int lastPress;
bool WasPressed = false;

//Menu
int selection=0;
int TimeRunning = 0;

void setup() {
  initialize();

}

void loop() {
  if (WasPressed)
    manageLastPress();

  
}

////////////////////////////////////////Functions defenition//////////////////////////////////////////

/////////////////////////////////Interruption Functions/////////////////////////

int pressed ()
{
  if (millis()-lastPress > 500)
  { 
    WasPressed = true;
    lastPress = millis();
  }
}
int moved ()
{
  if (millis()-lastPress > 50)
  {
    switch (selection){
      case 0:
        if (EncoderDirection() && rpm < 7200)
        {
          rpm += 100;
          DrawMenu(0);
        } else {
          if ( rpm > 0)
          {
            rpm -=100;
            DrawMenu(0);
          }
        }
      break;
      case 1:
        if (EncoderDirection() && _min < 1000)
        {
          _min += 1;
          DrawMenu(1);
        } else {
           if (_min > 1)
           {
            _min -= 1;
            DrawMenu(1);
           }
        }
    }
    lastPress = millis();
  }
}

ISR(TIMER2_OVF_vect){
  counter++; 
  if (counter > 3676)
  {
    if(_min!=1)
    {
      _min--;
      counter = 0;
      DrawMenu(2);
    } else {
      counter = 0;
      selection = 0;
      DrawMenu(0);
      pauseRotor();
      TIMER2OFF;
    }
  }
}
///////////////////////////////////General functions//////////////////////////////
void manageLastPress()
{
    int state;

    delay(500);
    
    state = digitalRead(2);
    
    if( (state == 0) && (selection == 0 || selection == 1))
    {
      if (selection == 0) //Spin
        spin();
      if (selection == 1) //Return to main Menu
      {
        selection = 0;
        DrawMenu(0); 
      }
    } else {   
      switch (selection)
      {
        case 0:
          DrawMenu(1);
          selection = 1;
        break;
        case 1:
          TIMER2ON;
          DrawMenu(2);
          selection = 2;
          startRotor(rpm, _min);
        break;
        case 2:
          pauseRotor();
          selection = 0;
          DrawMenu(0);
        break;
      }
    }
    WasPressed=false;
}
bool EncoderDirection () //TRUE formward FALSE backward
{
    if (digitalRead(encB) == LOW)
    {
      return true;  //Moved forward
    } else {
      return false; //Moved backward
    }
}


void startRotor (int rpm, unsigned int min)
{
  int objetive = map(rpm,0,7200,700,2000);
  for (int i = 700; i<objetive; i+=100)
  {
    ESC.writeMicroseconds(i);
    delay(500);
  }
  ESC.writeMicroseconds(objetive);
}

void pauseRotor()
{
  ESC.writeMicroseconds(700);
}

void DrawMenu (int _menu)
{
  switch (_menu)
  {
    case (0):
      u8g.firstPage();  
      do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 0, 22, String(String(rpm) + " rpm").c_str());
          u8g.setFont(u8g_font_6x10);
          u8g.drawStr( 0, 40, "Press to enter time");
          u8g.drawStr( 0, 52, "Hold for spin");
      } while( u8g.nextPage() );
      break;
    case(1):
        u8g.firstPage();
       do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 0, 22, String("Minuts: " + String(_min)).c_str());
          u8g.setFont(u8g_font_6x10);
          u8g.drawStr( 0, 40, "Press to start");
          u8g.drawStr( 0, 52, "Hold for cancel");
      } while( u8g.nextPage() );
      break;
    case(2):
      u8g.firstPage();
          do {
            u8g.setFont(u8g_font_unifont);
            u8g.drawStr( 0, 22, "RUNNING");
            u8g.drawStr( 0, 42, String(String(_min)+" mins left").c_str());
            u8g.setFont(u8g_font_6x10);
            u8g.drawStr( 0, 52, "Press to cancel");
          } while( u8g.nextPage() );  
      break;
  }
}

void spin ()
{
    int objetive = map(rpm,0,7200,700,2000);
    int i;
    while (digitalRead(2) == LOW)
    {
      if (i < objetive)
      {
        i += 100;
        ESC.writeMicroseconds(i);
      } else {
        ESC.writeMicroseconds(objetive);
      }
    }
}
////////////////////////////////////Initialization Function////////////////////////
void initialize ()
{
  Serial.begin(9600);


  ///////////////////////////Pin Initialization//////////////////////////////////
  //encoder
  pinMode (encA,INPUT);
  pinMode (encB,INPUT);
  pinMode(2, INPUT_PULLUP);
  //ESC
  pinMode(E_ESC, OUTPUT); 
  ///////////////////////////////////////////////////////////////////////////////

  u8g.firstPage();
       do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 0, 40, "Initializing...");
      } while( u8g.nextPage() );

  //////////////////////////////////ESC controller initialization//////////////// 
  //activate the MOSFET wich enable the ESC 
  digitalWrite (E_ESC, HIGH);

  delay(500);
  //ESC initialization sequence
  while (!ESC.attached())
  {
  ESC.attach(ESCpin);   
  delay(200);
  } 
  ESC.writeMicroseconds(2000); 
  delay(2000);
  ESC.writeMicroseconds(700); 
  delay(2000);
  ////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////Timer 2/////////////////////////////////////
    //Activación y preparación de la interrupcion del TIMER2 para que funcione a 1 Hz (una vez por segundo)
  TCCR2A = (1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1)| (0<<COM2B0) | (0<<3) | (0<<2) | (0<<WGM21) | (0<<WGM20);  ;   //modo normal, sin pwm, sin cuenta
  TCCR2B = (0<<FOC2A) | (0<<FOC2B) | (0<<5) | (0<<4) | (0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);
  TIMSK2 &= ~(1<<OCIE2A); //quitamos comparación, solo queremos por overflow
  // Oscilador interno
  ASSR = (0<<7) | (0<<EXCLK) | (0<<AS2) | (0<<TCN2UB) | (0<<OCR2AUB) | (0<<OCR2BUB) | (0<<TCR2AUB) | (0<<TCR2BUB) ;
  // configuramos un prescaler de 1024
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  ////////////////////////////////////////////////////////////////////////////////
    
  //////////////////The start/////////////////
  DrawMenu(0);
  EIFR  = bit (INTF0); // clearing button pushed before the menu drawing
  
  /////////////////////////Interruptions initialization///////////////////////////
  attachInterrupt(digitalPinToInterrupt(2), pressed,FALLING); //enconder have been pressed
  attachInterrupt(digitalPinToInterrupt(3), moved,LOW); //enconder have been rotated
  ////////////////////////////////////////////////////////////////////////////////

}

