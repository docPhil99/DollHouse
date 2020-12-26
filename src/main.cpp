#include <Arduino.h>
#include <Keypad.h>

const int NUM_LEDS=7; //number of LEDS
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};
byte rowPins[ROWS] = {6, 5, 4, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 2, 7}; //connect to the column pinouts of the keypad
byte ledPins[]={10,11,12,13,A0,A1,A2}; //pins for LEDS


Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
byte ledState[]={0,0,0,0,0,0,0};
unsigned long loopCount = 0;
unsigned long timer_t = 0;
byte zoom_counter=0;

void setup(){
	Serial.begin(9600);
	
	// Try playing with different debounceTime settings to see how it affects
	// the number of times per second your loop will run. The library prevents
	// setting it to anything below 1 millisecond.
	kpd.setDebounceTime(10);	// setDebounceTime(mS)
  for(int i=0;i<NUM_LEDS;i++)
  {
     pinMode(ledPins[i], OUTPUT);
  }
}

enum run_mode{single,party,zoom,off,allon, flash};

enum run_mode mode=single;
int val=0;
unsigned long previousMillis = 0;        // will store last time LED was updated
bool timeout = 0;
// constants won't change:
const long interval = 100; 

void loop(){
	char key = kpd.getKey();
	
	// Report the number of times through the loop in 1 second. This will give
	// you a relative idea of just how much the debounceTime has changed the
	// speed of your code. If you set a high debounceTime your loopCount will
	// look good but your keypresses will start to feel sluggish.
	if ((millis() - timer_t) > 1000) {
		Serial.print("Your loop code ran ");
		Serial.print(loopCount);
		Serial.println(" times over the last second");
		loopCount = 0;
		timer_t = millis();
	}
	loopCount++;
	if(key)
  {
      
    if(key=='#')
    {
      Serial.println("party");
      mode=party;
    }
    else if(key=='*')
    {
        mode=zoom;
        Serial.println("zoom");
      
    }
    else
    {
      val = atoi(&key);
      if (val == 0)
      {
        mode = off;
      }
      else if(val ==8)
      {
        mode = flash;
      }
      else if(val ==9)
      {
        mode = allon;
      }
      else
      {
        mode = single;
        ledState[val-1]=~ledState[val-1];
        Serial.println("***************************************************************************");
      }
    }
  }
  unsigned long currentMillis = millis();
  timeout = currentMillis - previousMillis >= interval;
  if(timeout)
    previousMillis=currentMillis;

  if(mode==off)
  {
    for(int i=0;i<NUM_LEDS;i++)
      ledState[i]=0;
  }
  else if(mode==allon)
  {
    for(int i=0;i<NUM_LEDS;i++)
      ledState[i]=1;
  }
  else if(mode==party && timeout)
  {
    for(int i=0;i<NUM_LEDS;i++)
      ledState[i]=0;
      ledState[random(0,NUM_LEDS)]=1;
  }
  else if(mode==flash && timeout)
  {
    for(int i=0;i<NUM_LEDS;i++)
      ledState[i]=~ledState[i];
      
  }
  else if(mode==zoom & timeout)
  {
    for(int i=0;i<NUM_LEDS;i++)
      ledState[i]=0;
    ledState[zoom_counter]=1;
    zoom_counter++;
    if (zoom_counter>=NUM_LEDS)
      zoom_counter=0;
  }
  //set LEDS states
  for(int i=0;i<NUM_LEDS;i++)
  {
      Serial.print(ledState[i]);
      digitalWrite(ledPins[i],ledState[i]);

  }
  Serial.print(val);
  Serial.println(mode);
}

