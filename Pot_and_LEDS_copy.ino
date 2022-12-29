/*

  Code to turn on a bank of LEDS based on the position of a potentiometer
  Mode 0: Sequential
  R - G - B - R - G - B - R - G _ B
  Mode 1: Cascade
  R - RG - RGB - RG - R - RG - RGB - RG - R
  Mode 2: Fade RGB
  R - G - B - R - G - B
  Mode 3: Fade Sequential
  
  
  1. Wave
  2. Sequential
  3. Slow Glow
  4. Chasing
  5. Flash
  6. Twinkle
  7. All On
  8. All Off

  (C)SGF2022

*/

#include <WiFiNINA.h>           // needed for on board LEDs

// analog pins to allow fading
#define POTENTIOMETER_PIN A0    // connect the swipe pin of a potentiometer to pin AO
#define LED_RED 15    // A1
#define LED_GREEN 16  // A2
#define LED_BLUE 17   // A3

#define time_delay 250               // delay between LEDs

byte led = B10010;

int ledPins[] = { LED_RED, LED_GREEN, LED_BLUE};
String modes[] = {"Sequential","Cascade","Fade Sequential","Twinkle","Flash","Twinkle","All ON","All OFF"};

int ledCount = 2; // 0, 1, 2

int brightness = 0;    // how bright the LED is
int fadeBrightStep = 10;    // how many steps to fade the LED by
int fadeDimStep = 15;
int fadeDelay = 20;    // ms delay between fades

void setup() {

  // define external LEDs
  for (int i = 0; i < ledCount ; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  turnOffAllLEDS();

  //define builtin LED. The constant LED_BUILTIN is a built-in constant name
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //define the on-board RGB LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  //initialise the monitor
  Serial.begin(9600);

}

void loop() {

  // get the potentiometer value and map it
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int mode = map(potentiometerValue, 0, 1023, 0, 4); // change the final number as more selections become available

  Serial.println("Mode: " + String(mode) + ":" + modes[mode]);

  delay(100);

  // select the code to run based on the value of mode that is returned by the map function
  switch (mode) { 
    case 0: // Sequential

      // go up the LEDS on then off then back down again
      // control the external LEDs
      turnOnRGB_RED();
      turnOnLED(LED_RED, time_delay);
      turnOffRGB();
      turnOffLED(LED_RED, time_delay); 
      
      turnOnRGB_GREEN();
      turnOnLED(LED_GREEN, time_delay);
      turnOffRGB();
      turnOffLED(LED_GREEN, time_delay);
      
      turnOnRGB_BLUE();
      turnOnLED(LED_BLUE, time_delay);
      turnOffRGB();
      turnOffLED(LED_BLUE, time_delay);

      break;

    case 1: // Cascade
      // Bring LEDs one by one


      turnOnLED(LED_RED, time_delay);
      turnOnLED(LED_GREEN, time_delay);
      turnOnLED(LED_BLUE, time_delay);
      
      // Turn off LEDs one by one
      
      turnOffLED(LED_BLUE, time_delay);
      turnOffLED(LED_GREEN, time_delay);
      turnOffLED(LED_RED, time_delay);    

      break;
    
    case 2: // Fade
    
      //fade on and off LED
      delay(100);
      fadeOnLED(LED_RED, fadeDelay);
      fadeOnLED(LED_GREEN, fadeDelay);
      fadeOnLED(LED_BLUE, fadeDelay);
      turnOffAllLEDS();
      
      break;
      
    case 3: // Fade R - RG - RGB - RG - R

      fadeOnLED(LED_RED, fadeDelay);
      fadeOnLED(LED_GREEN, fadeDelay);
      fadeOnLED(LED_BLUE, fadeDelay);
      
      fadeOffLED(LED_BLUE, fadeDelay);
      fadeOffLED(LED_GREEN, fadeDelay);
      fadeOffLED(LED_RED, fadeDelay);
      
    break;
      

    default:
      //digitalWriteRGB(LOW, LOW, LOW);
      //Serial.println("Mode default");
    break;
  }
}

/***************************************

  functions to control the onboard RGB LED

****************************************/

void turnOffRGB() {

  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);

}

void turnOnRGB_RED() {

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);

}

void turnOnRGB_GREEN() {

  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);

}

void turnOnRGB_BLUE() {

  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);

}

void turnOnRGB_YELLOW() {

  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

}

void turnOnRGB_WHITE() {

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

}

void turnOffAllLEDS(){
  
  for (int j = 0; j <= ledCount; j++) {
    
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Pin: " + String(ledPins[j]) + " ALL OFF");
    analogWrite(ledPins[j], 0);
    
  }

}

void fadeOnLED(int LED_PIN, int delay_time){
  
  // brighten LEDS
  Serial.println("Pin: " + String(LED_PIN) + " BRIGHTENING");
  for (int brightness = 0; brightness <= 255; brightness = brightness + fadeBrightStep){
    analogWrite(LED_PIN, brightness);
    delay(fadeDelay);
  }
  
}

void fadeOffLED(int LED_PIN, int delay_time){
   // dim ledPins[j]
  Serial.println("Pin: " + String(LED_PIN) + " DIMMING");
  for (int brightness = 255; brightness >=0; brightness = brightness - fadeDimStep){
    analogWrite(LED_PIN, brightness);
    delay(fadeDelay);
  }  
  
}

void turnOnLED(int LED_PIN, int delay_time){
  
  Serial.println("Pin: " + String(LED_PIN) + " ON");
  analogWrite(LED_PIN, 255);
  delay(delay_time);
  
}

void turnOffLED(int LED_PIN, int delay_time){
  
  Serial.println("Pin: " + String(LED_PIN) + " OFF");
  analogWrite(LED_PIN, 0);
  delay(delay_time);
  
  
}

