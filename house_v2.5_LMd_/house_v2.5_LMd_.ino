#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OPTIONS
#define LIGHTS  1
#define ALARMS  2
#define TEMP 3
#define KEY 4
#define MODES 5
#define INFO 6

// LIGHTS (case values & pin numbers)
#define KITCHEN 2
#define LIVING 3
#define DINING 4
#define ROOM1 5
#define ROOM2 6
#define ALLOFF 7

// MODES 
#define SLEEP 1
#define WAKEUP 2
#define OUT 3
#define TEST 4

#define BACK -1

// DISPLAY (DO NOT MOVE)
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int timerInterruptions = 0; 
int timerAlarm = 0; 

void setup() {
  // put your setup code here, to run once:

  // Serial
  Serial.begin(9600); 
  while(!Serial); 
  
  // LEDS configuration
  pinMode(KITCHEN, OUTPUT); 
  pinMode(LIVING, OUTPUT); 
  pinMode(DINING, OUTPUT); 
  pinMode(ROOM1, OUTPUT);
  pinMode(ROOM2, OUTPUT);  

  // Display configuration
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // I2C addr 0x3C (for the 128x64)  
  display.display();
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
     
   case ALARMS: 
    break; 

    case TEMP: 
      break; 

    case KEY: 
      break; 

    case MODES:
      Serial.println("MODES!");
      selectMode();
      break; 

    case INFO: 
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
       case ROOM1:
        digitalWrite(ROOM1, !digitalRead(ROOM1)); 
        break; 
       case ROOM2:
        digitalWrite(ROOM2, !digitalRead(ROOM2));
        break; 
       case ALLOFF: 
        digitalWrite(KITCHEN, LOW);
        digitalWrite(LIVING, LOW);
        digitalWrite(DINING, LOW);
        digitalWrite(ROOM1, LOW);
        digitalWrite(ROOM2, LOW);
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
        //activateAlarms(); 
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
  digitalWrite(ROOM1, LOW);
  digitalWrite(ROOM2, LOW);
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
    digitalWrite(ROOM1, HIGH);
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
