#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// OPTIONS
#define LIGHTS  1
#define LOCK  2
#define TEMP 3
#define KEY 4
#define MODES 5

// LIGHTS (case values & pin numbers)
#define KITCHEN 2
#define LIVING 8
#define DINING 4
#define ROOM 5
#define ALLOFF 20

// MODES 
#define SLEEP 1
#define WAKEUP 2
#define OUT 3
#define TEST 4

// TEMP OPT 
#define INCTEMP 1
#define DECTEMP 2

// UNLOCK
#define UNLOCKING 1
#define ALARM 6

#define BACK -1

// DISPLAY (DO NOT MOVE)
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


// TIMER INTERRUPTION (ALARM)
int timerInterruptions = 0; 
int timerAlarm = 0; 

// TEMP SENSOR
#define TEMPPIN 7
OneWire oneWire(TEMPPIN);
DallasTemperature sensors(&oneWire);
#define AIRC 3

// LOCKBOCK PASSWORD
#define PASSWORD 12345

void setup() {
  // Serial
  Serial.begin(9600); 
  while(!Serial); 
  
  // LEDS configuration
  pinMode(KITCHEN, OUTPUT); 
  pinMode(LIVING, OUTPUT); 
  pinMode(DINING, OUTPUT); 
  pinMode(ROOM, OUTPUT);

  // Display configuration
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // I2C addr 0x3C (for the 128x64)  
  display.display();

  // Temperature sensor configuration 
   sensors.begin();
   pinMode(AIRC, OUTPUT); 
   turnOffAllLights();

   // Lockbox alarm configuration
   pinMode(ALARM, OUTPUT);
}

////////////////////////////////////////////////////////////////////////////////////// MAIN

void loop() {
  int button = 0; 
  while(1){
    Serial.println("Waiting for button");
    button = Serial.parseInt();
    if(button != 0)
    {
      break; 
    }
  }
  Serial.print("Button ");
  Serial.println(button);
  switch(button) {
    case LIGHTS:
      Serial.println("LIGHTS!");
      turnOnOffLights(0); 
      break; 
     
   case LOCK:
    Serial.println("LOCK!");
    lockBox();
    break; 

    case TEMP:
      Serial.println("TEMP!");
      readSendTemp(); 
      break; 

    case KEY: 
      break; 

    case MODES:
      Serial.println("MODES!");
      selectMode();
      break; 

    default: 
      break; 
  }
}

////////////////////////////////////////////////////////////////////////////////////// LIGHTS

void turnOnOffLights(int light) {
  lights: 
    while(1){
      Serial.println("WAITING FOR LIGHT!");
      light = Serial.parseInt(); 
      if(light != 0) {
        break;
      }
    }
    Serial.print("LIGHT:"); 
    Serial.println(light);
    
    switch(light) {
      case KITCHEN:
         digitalWrite(KITCHEN, !digitalRead(KITCHEN));
         break; 
       case LIVING:
        digitalWrite(LIVING, !digitalRead(LIVING));
        break; 
       case DINING: 
        digitalWrite(DINING, !digitalRead(DINING));
        break; 
       case ROOM:
        digitalWrite(ROOM, !digitalRead(ROOM)); 
        break; 
       case ALLOFF: 
        digitalWrite(KITCHEN, LOW);
        digitalWrite(LIVING, LOW);
        digitalWrite(DINING, LOW);
        digitalWrite(ROOM, LOW);
        break; 
       case BACK: 
        return; 
       default:
        break; 
    }
    goto lights;
}


////////////////////////////////////////////////////////////////////////////////////// MODES

void selectMode() {
  modes: 
    String modeString = "";
    int mode = 0; 
    while(1){
      Serial.println("WAITING FOR MODE!");
      mode = Serial.parseInt(); 
      if(mode != 0) {
        break;
      }
    }
    Serial.print("MODE:"); 
    Serial.println(mode);
    
    switch(mode) {
      case SLEEP:
        turnOffAllLights();
        modeString = "SLEEP";
        break; 
       case WAKEUP:
        setTimer(); 
        modeString = "WAKEUP";
        break; 
       case OUT:
        turnOffAllLights();
        digitalWrite(ALARM, HIGH);
        digitalWrite(ALARM, HIGH);
        delay(200); 
        digitalWrite(ALARM, LOW);
        digitalWrite(ALARM, LOW);
        Serial.write("WINDOWS AND DOOR CLOSE");
        modeString = "OUT";
        break; 
       case TEST:
        modeString = "TEST";
        break;
       case BACK:
        return;  
       default: 
        break;
    }
    OLEDdisplay(modeString); 
  goto modes;
}

void turnOffAllLights() {
  digitalWrite(KITCHEN, LOW);
  digitalWrite(LIVING, LOW);
  digitalWrite(DINING, LOW);
  digitalWrite(ROOM, LOW);
}

void setTimer(){
  timerAlarm = 0; 
  while(1){
    Serial.println("WAITING FOR TIMER ALARM VALUE!");
    timerAlarm = Serial.parseInt(); 
    if(timerAlarm != 0) {
      break;
    }
  }
  Serial.print("TIMER ALARM VALUE:"); 
  Serial.println(timerAlarm);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCNT1 = 34286;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

// ISR every second
ISR(TIMER1_OVF_vect)       
{
  timerInterruptions++; 
  if (timerInterruptions == timerAlarm)
  {
    Serial.print("INTERRUPTION ALARM!");
    digitalWrite(ROOM, HIGH);
    noInterrupts();
    timerInterruptions = 0;
    TCCR1A = 0; 
    TCCR1B = 0; 
  }
}

void OLEDdisplay(String text) {
  // Clear the buffer.
  display.clearDisplay();

  // Text display
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(text);
  display.display();

  // Turn off screen
  //display.clearDisplay();
  //display.display();
}

////////////////////////////////////////////////////////////////////////////////////// TEMP

void readSendTemp() {  
  //Simulated temp
  sensors.requestTemperatures();
  int tempSim = sensors.getTempCByIndex(0); 
  
  temps: 
    int tempOpt = 0;
    while(1){
      Serial.println("WAITING FOR TEMP OPTION!");
      tempOpt = Serial.parseInt();
      
      // Read and send temperature
      sensors.requestTemperatures();
      Serial.write((int)sensors.getTempCByIndex(0));
      Serial.println(sensors.getTempCByIndex(0));
      
      if(tempOpt != 0) {
        break;
      }
    }
    Serial.print("TEMP OPTION:"); 
    Serial.println(tempOpt);
    
    switch(tempOpt) {
      case INCTEMP:
        tempSim += 30; 
        tempSim = (tempSim>255) ? 255 : tempSim; 
        analogWrite(AIRC, tempSim); 
        break; 
       case DECTEMP:
        tempSim-=30;
        tempSim = (tempSim<0) ? 0 : tempSim; 
        analogWrite(AIRC, tempSim);
        break;  
       case BACK:
        return;  
       default: 
        break;
    }
  goto temps;
}

////////////////////////////////////////////////////////////////////////////////////// LOCK

void lockBox() {
  int attempts = 3;
  locks:
    int pwd = 0;
    while(1){
        Serial.println("WAITING FOR UNLOCK!");
        pwd = Serial.parseInt(); 
        if(pwd != 0) {
          break;
        }
      }
      Serial.print("PWD:"); 
      Serial.println(pwd);

      if (pwd == -1) {
        return;
      }
      else {
        if (pwd == PASSWORD) {
          Serial.println("Correct password!"); 
          digitalWrite(ALARM, HIGH);
          digitalWrite(ALARM, HIGH);
          delay(100); 
          digitalWrite(ALARM, LOW);
          digitalWrite(ALARM, LOW);
          return;
        }
        else {
          Serial.println("Wrong password!");  
          if (attempts == 0) {
            digitalWrite(ALARM, HIGH);
            digitalWrite(ALARM, HIGH);
            delay(1000); 
            digitalWrite(ALARM, LOW);
            digitalWrite(ALARM, LOW);
            Serial.write("ATTEMPTS_EXCEEDED");
            return;
          }
          attempts--;
        }
      }
    goto locks;
}
