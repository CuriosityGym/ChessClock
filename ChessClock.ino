//We always have to include the library
#include "LedControl.h"
#include "font.h"
#include <Adafruit_NeoPixel.h>
#include<CountUpDownTimer.h>

#define PIXELPIN 4
#define NUMPIXELS 1
#define MODESTANDARD 0
#define MODEBLITZ 1
#define MODEBULLET 2
#define MODERAPID 3

#define MODECOUNT 4
#define WAITDELAYMODESET 5000


CountUpDownTimer LEFT(DOWN);
CountUpDownTimer RIGHT(DOWN);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);



uint8_t modeButton=6;
uint8_t leftButton=7;
uint8_t rightButton=8;

uint8_t mode;
boolean modeSet=false;
uint8_t selectedMode;
boolean hasAnyPlayerPlayed=false;

uint8_t currentMode=0;


#define NUMDISP 4
LedControl lc=LedControl(12,11,10,4);
byte final[8]={};
boolean blinkShown=false;


unsigned long lastPressedAt;

void setup() {

Serial.begin(115200);
pinMode(modeButton, INPUT_PULLUP);
 pinMode(leftButton, INPUT_PULLUP);
 pinMode(rightButton, INPUT_PULLUP);
for(int a=0;a<NUMDISP;a++)
  {
    lc.shutdown(a,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(a,0);
    /* and clear the display */
    lc.clearDisplay(a);
    //byte final[8]={};
  }

  mode=0;//set to standard Mode 
 for(int f=0;f<8;f++)
  {
      lc.setRow(3,f,MO[f]);
      lc.setRow(2,f,DE[f]);
  }
  setDisplay(1,currentMode+1);

  while(!modeSet)
{
    setMode();
  
}

switch(selectedMode)
{
  case 0: // Bullet Mode, 1min
        LEFT.SetTimer(0,1,1);
        RIGHT.SetTimer(0,1,1);
        LEFT.StopTimerAt(0,0,0);
        RIGHT.StopTimerAt(0,0,0);  
        LEFT.StartTimer();
        RIGHT.StartTimer();
        break;

  case 1: //Blitz Mode, 3mins
        LEFT.SetTimer(0,3,1);
        RIGHT.SetTimer(0,3,1);
        LEFT.StopTimerAt(0,0,0);
        RIGHT.StopTimerAt(0,0,0);  
        LEFT.StartTimer();
        RIGHT.StartTimer();
        break;

        
   case 2: //Rapid Mode, 25mins
        LEFT.SetTimer(0,25,1);
        RIGHT.SetTimer(0,25,1);
        LEFT.StopTimerAt(0,0,0);
        RIGHT.StopTimerAt(0,0,0);  
        LEFT.StartTimer();
        RIGHT.StartTimer();
        break;

   case 3: //Rapid Mode, 3mins
        LEFT.SetTimer(1,30,1);
        RIGHT.SetTimer(1,30,1);
        LEFT.StopTimerAt(0,0,0);
        RIGHT.StopTimerAt(0,0,0);  
        LEFT.StartTimer();
        RIGHT.StartTimer();
        break;     
  
}

    lc.clearDisplay(3);
    lc.clearDisplay(1);
    int p;
    for(p=5;p>0;p--)
    {
       
      setDisplay(2,p);      
      setDisplay(0,p);
      delay(1000);

    
    
    }

    for(int d=0;d<8;d++)
    {

      lc.setRow(2,d,GO[d]);
      lc.setRow(0,d,GO[d]);
    }
    delay(2000);


  displayLeftPlayerTime();
  displayRightPlayerTime();  
  LEFT.PauseTimer(); 
 // RIGHT.PauseTimer();





/*
  while (!hasAnyPlayerPlayed)

  {

        if(digitalRead(rightButton)!=LOW)
        {
            hasAnyPlayerPlayed=true;
            LEFT.PauseTimer(); 
            RIGHT.ResumeTimer();
        }

        if(digitalRead(leftButton)!=LOW)
        {
            hasAnyPlayerPlayed=true;
            RIGHT.PauseTimer(); 
            LEFT.ResumeTimer();
        }
  }

  */

  
  
}

void loop() 
{
    
    displayLeftPlayerTime();
    displayRightPlayerTime();  
     
       if(digitalRead(rightButton)==LOW)
        {
            hasAnyPlayerPlayed=true;
            LEFT.PauseTimer(); 
            RIGHT.ResumeTimer();
           // leftPlayerPlaying=false;
        }

        if(digitalRead(leftButton)==LOW)
        {
            hasAnyPlayerPlayed=true;
            RIGHT.PauseTimer(); 
            LEFT.ResumeTimer();
            //leftPlayerPlaying=false;
        }
     
        
 


  



    
 

}

void setMode()
{

  boolean modeSwitchState=digitalRead(modeButton);
  
  if(modeSwitchState==LOW)//button is pressed
  {
    lastPressedAt=millis();
    delay(300);    
    mode=mode+1;
    currentMode=mode%MODECOUNT;
    Serial.print("Mode Clicked: ");
    Serial.println(mode);
    setDisplay(1,currentMode+1); 
     
      
  }
  if((millis()-lastPressedAt)>WAITDELAYMODESET)
  {
    modeSet=true;// The Mode switch has not been pressed since 5 seconds
    selectedMode=currentMode;
    Serial.print("selectedMode: ");
    Serial.println(selectedMode);
  }
}

void displayLeftPlayerTime()
{

  LEFT.Timer();// run the timer

  if (LEFT.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
        blinkShown=!blinkShown;
        if(selectedMode==0)
        {
          setDisplay(1,LEFT.ShowMinutes());
          setDisplay(0,LEFT.ShowSeconds());
        } 
        if(selectedMode==1)
        {
          setDisplay(1,LEFT.ShowMinutes());
          setDisplay(0,LEFT.ShowSeconds());
        } 
        if(selectedMode==2)
        {
          setDisplay(1,LEFT.ShowMinutes());
          setDisplay(0,LEFT.ShowSeconds());
        } 
        if(selectedMode==3)
        {
          setDisplay(1,LEFT.ShowHours());
          setDisplay(0,LEFT.ShowMinutes());
        }       
        blinkSeconds(0,blinkShown);
        blinkSeconds(1,!blinkShown);
       


  }

  




}

void displayRightPlayerTime()
{

  RIGHT.Timer();// run the timer

  if (RIGHT.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
      blinkShown=!blinkShown;
      if(selectedMode==0)
        {
           setDisplay(3,RIGHT.ShowMinutes());
           setDisplay(2,RIGHT.ShowSeconds()); 

        } 
        if(selectedMode==1)
        {
          setDisplay(3,RIGHT.ShowMinutes());
          setDisplay(2,RIGHT.ShowSeconds()); 

        } 
        if(selectedMode==2)
        {
          setDisplay(3,RIGHT.ShowMinutes());
          setDisplay(2,RIGHT.ShowSeconds()); 

        } 
        if(selectedMode==3)
        {
           setDisplay(3,RIGHT.ShowHours());
           setDisplay(2,RIGHT.ShowMinutes()); 

        }   
     
      
      blinkSeconds(3,blinkShown);
      blinkSeconds(2,!blinkShown);  
  }

 
  
   

}



void blinkSeconds(uint8_t displayNumber, boolean showValue)
{
  if(showValue)
  {
    lc.setRow(displayNumber,0,0xFF);
    lc.setRow(displayNumber,1,0x00);
  }
  else
  {
    lc.setRow(displayNumber,0,0x00);
    lc.setRow(displayNumber,1,0xFF);
   
  }

    

}


void setDisplay(uint8_t displayNumber, uint8_t value)
{
  byte displayData[8]={};
  uint8_t unitsDigit=0;
  uint8_t tensDigit=0;
  
  if(value<10)
  {
     tensDigit=0; 
     unitsDigit=value;
  }

  if(value>9)
  {
     tensDigit=floor(value/10); 
     unitsDigit=value%10;
  }
  
   for(int i=0;i<8;i++)
  {
     displayData[i]=0x00;
     switch (unitsDigit) 
     {
      case 0: displayData[i]= zero[i];break;
      case 1: displayData[i]= one[i];break;
      case 2: displayData[i]= two[i];break;
      case 3: displayData[i]= three[i];break;
      case 4: displayData[i]= four[i];break;
      case 5: displayData[i]= five[i];break;
      case 6: displayData[i]= six[i];break;
      case 7: displayData[i]= seven[i];break;
      case 8: displayData[i]= eight[i];break;
      case 9: displayData[i]= nine[i];break;
      
     }
     displayData[i]=displayData[i]>>4;
     
    /* if(secondDigit==0)
     {
          displayData[i]= zero[i];
     } 
     displayData[i]= (four[i] | displayData[i]>>4)>>1 ;
     */
  }

   for(int i=0;i<8;i++)
  {
     //displayData[i]=0x00;
     switch (tensDigit) 
     {
      case 0: displayData[i]= zero[i] | displayData[i]; break;
      case 1: displayData[i]= one[i] | displayData[i]; break;
      case 2: displayData[i]= two[i] | displayData[i]; break;
      case 3: displayData[i]= three[i] | displayData[i]; break;
      case 4: displayData[i]= four[i] | displayData[i]; break;
      case 5: displayData[i]= five[i] | displayData[i]; break;
      case 6: displayData[i]= six[i] | displayData[i]; break;
      case 7: displayData[i]= seven[i] | displayData[i]; break;
      case 8: displayData[i]= eight[i] | displayData[i]; break;
      case 9: displayData[i]= nine[i] | displayData[i]; break;
      
     }

     displayData[i]=displayData[i]>>1;
  } 

    lc.setRow(displayNumber,0,displayData[0]);
    lc.setRow(displayNumber,1,displayData[1]);
    lc.setRow(displayNumber,2,displayData[2]);
    lc.setRow(displayNumber,3,displayData[3]);
    lc.setRow(displayNumber,4,displayData[4]);
    lc.setRow(displayNumber,5,displayData[5]);
    lc.setRow(displayNumber,6,displayData[6]);
    lc.setRow(displayNumber,7,displayData[7]);

}


