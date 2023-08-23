#include <TeensyThreads.h>//richardsito te pido perdon
#include "Nextion.h"

int bProximity=30;
int fProximity=31;
int sPushGo=33;
int sPushBack=34;

int relay1 = 2;
int relay2 = 3;
int relay3 = 4;
int relay4 = 5;

int warningBuzzer=37;

int blueLED=7;
int redLED=8;
int greenLED=9;

int state=0;

int runNow=0;

int ledStatus=0;
int alarmStatus=0;
int id1;
int id2;
int id3;
int id4;
int id5;

NexPage introPage = NexPage(1,0,"Intro");
NexPage mainPage = NexPage(2,0,"Main");
NexPage counterPage =NexPage(3,0,"Counter");
NexPage goPage = NexPage(4,0,"Go");
NexPage runningPage = NexPage(5,0,"Running");

NexButton startB = NexButton(2,1,"startb");

void startBPushCallback(void *ptr)
{
 runNow=1;
}

NexTouch *nex_listen_list[] =
{
 &startB,
 NULL
};

void listener()
{
 while(1)
 {
  nexLoop(nex_listen_list);
  threads.yield();
 }
}


void nextionInit()
{
 nexInit();
 startB.attachPush(startBPushCallback, &startB);
 threads.delay(500);
 introPage.show(); 
}




void setup() 
{
 pinMode(sPushGo,INPUT);
 pinMode(sPushBack,INPUT);
 pinMode(fProximity,INPUT);
 pinMode(bProximity,INPUT);

 pinMode(relay1,OUTPUT);
 pinMode(relay2,OUTPUT);
 pinMode(relay3,OUTPUT);
 pinMode(relay4,OUTPUT);

pinMode(warningBuzzer,OUTPUT);
 pinMode(greenLED,OUTPUT);
 pinMode(redLED,OUTPUT);
 pinMode(blueLED,OUTPUT);

digitalWrite(warningBuzzer,LOW);
 ledStatus=2;
 alarmStatus=0;
 
 threads.setMicroTimer(1);
 id1 = threads.addThread(nextionInit);
 id2 = threads.addThread(listener);
 id3 = threads.addThread(statesMachine);
 id4 = threads.addThread(Leds);
 id5 = threads.addThread(alarm);
}

bool alarmBool=false;

void alarm()
{
 while(1)
 {
  if(alarmStatus==1)
  {
   if(alarmBool)
   {
    digitalWrite(warningBuzzer,HIGH);
     threads.delay(75);
   }
   else
   {
    digitalWrite(warningBuzzer,LOW);
     threads.delay(1250);
   }
   alarmBool=!alarmBool;
  }
  
  else
  {
   digitalWrite(warningBuzzer,LOW);
  }
 
  
  threads.yield();
 }
}

void Leds()
{
 while(1)
 {
  if(ledStatus==1)
  {
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);
    digitalWrite(blueLED,LOW);
  }
  else if(ledStatus==2)
  {
   digitalWrite(redLED,HIGH);
   digitalWrite(greenLED,LOW);
   digitalWrite(blueLED,LOW);
  }
  else if(ledStatus==3)
  {
    digitalWrite(blueLED,HIGH);
    digitalWrite(greenLED,LOW);
    digitalWrite(redLED,LOW);
  }
  else
  threads.yield();
 }  
}

void backward()
{
 digitalWrite(relay1,HIGH);
 digitalWrite(relay2,LOW);
 digitalWrite(relay3,HIGH);
 digitalWrite(relay4,LOW);
}

void forward()
{
 digitalWrite(relay1,LOW);
 digitalWrite(relay2,HIGH);
 digitalWrite(relay3,LOW);
 digitalWrite(relay4,HIGH); 
}

void motorStop()
{
  digitalWrite(relay1,HIGH);
 digitalWrite(relay2,HIGH);
 digitalWrite(relay3,LOW);
 digitalWrite(relay4,LOW); 

 threads.delay(100);
 digitalWrite(relay1,LOW);
 digitalWrite(relay2,LOW);
 digitalWrite(relay3,LOW);
 digitalWrite(relay4,LOW); 
}


void statesMachine()
{
 while(1)
 {
  switch(state)
  {
   case 0:
   {
    if(digitalRead(sPushGo)==0)
    {
     unsigned long pushed = millis();
     while(digitalRead(sPushGo)==0)
     {
      if((millis()-pushed)>500)
      {
       state=1;
       break;
      }
      else
      {
       state=0;
      }
     }
    }
    else if(digitalRead(sPushBack)==0)
    {
     unsigned long pushed = millis();
     while(digitalRead(sPushBack)==0)
     {
      if((millis()-pushed)>500)
      {
       state=3;
       break;
      }
      else 
      {
       state=0;
      }
     }
    }
    else if(runNow==1)
    {
      ledStatus=1;
       counterPage.show();
      threads.delay(5000);
      alarmStatus=1;
     forward();
    
     runNow=0;
     ledStatus=3;
     state=2;
    }  
   }
   break;
  
  case 1:
  {
   if(digitalRead(sPushGo)==1)
   { 
    
    counterPage.show();
    ledStatus=1;
    threads.delay(5000);
    ledStatus=3;
    alarmStatus=1;
    forward();
    state=2;
   }
  }
  break;

  case 2:
  {
   if(digitalRead(fProximity)==0)
   {
    threads.delay(500);
    motorStop();
    ledStatus=2;
    alarmStatus=0;
    mainPage.show();
    state=0;
   }
  }
  break;

  case 3:
  {
   if(digitalRead(sPushBack)==1)
   {
    state=4;    
    counterPage.show();
    ledStatus=1;
    threads.delay(5000);
    ledStatus=3;
    alarmStatus=1;
    backward();
   }
  }
  break;
  
  case 4:
  {
   if(digitalRead(bProximity)==0)
   {
    threads.delay(500);
    motorStop();
    ledStatus=2;
    alarmStatus=0;
    mainPage.show();
    state=0;
   }
  }
  break;
 }
 threads.yield();
 }
}

void loop() 
{ 
}
