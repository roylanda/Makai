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

#define BACK -1

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

}

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
      turnOnOffLights(); 
      break; 
     
   case ALARMS: 
    break; 

    case TEMP: 
      break; 

    case KEY: 
      break; 

    case MODES: 
      break; 

    case INFO: 
      break; 

    default: 
      break; 
  }
}

void turnOnOffLights() {
  lights:
    int light = 0; 
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

int readBth() {
  if (Serial.available()) {
    return Serial.read(); 
  }
  else {
    return -1; 
  }
}

int selectMode(){
  
}
