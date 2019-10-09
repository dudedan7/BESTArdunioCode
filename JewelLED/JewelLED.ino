#include <Adafruit_NeoPixel.h>

#define PIN 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, PIN);

uint8_t  mode   = 0, // Current animation effect
         offset = 0;
uint32_t color  = 0XFFFFFF; // Starting color
uint32_t prevTime; 

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(40); // 1/3 brightness
  prevTime = millis();
}

void loop() {

  if(Serial.available()>0)
   {     
      char data= Serial.read(); // reading the data received from the bluetooth module
      //String junk = Serial.readString();
      switch(data)
      {
        case '0': mode=0;Serial.println("Mode changed: 0");break; 
        
        case '1': mode=1;Serial.println("Mode changed: 1");break;
        case '2': mode=2;Serial.println("Mode changed: 2");break; 
        default : break;
      }
      
   }
  
  uint8_t  i;
  uint32_t t;

  switch(mode) {
    
   case 0: //rainbow hold
    holdColor(1000);
    delay(0);
    break;
    
   case 1: //rainbow cycle slow
    rainbowCycleslow(20);
    delay(0);
    break;
       
   case 2: //rainbow cycle fast 
    rainbowCycle(0);
    delay(0);
    break;
  }


  
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void holdColor(uint8_t wait) {
  uint16_t r, j, k;
    for (r=0; r<=6; r++){
    pixels.setPixelColor(r, pixels.Color(50,0,0));
    }
    pixels.show();
    for(k=0;k<10;k++){
    delay(wait);
    if (Serial.available()>0) {
      return;
    }
    }
    
    for (r=0; r<=6; r++){
    pixels.setPixelColor(r, pixels.Color(0,50,0));
    }
    pixels.show();
    for(k=0;k<10;k++){
    delay(wait);
    if (Serial.available()>0) {
      return;
    }
    }
    
    for (r=0; r<=6; r++){
    pixels.setPixelColor(r, pixels.Color(0,0,50));
    }
    pixels.show();
    for(k=0;k<10;k++){
    delay(wait);
    if (Serial.available()>0) {
      return;
    }
    }
    
    for (r=0; r<=6; r++){
    pixels.setPixelColor(r, pixels.Color(50,50,50));
    }
    pixels.show();
    for(k=0;k<10;k++){
    delay(wait);
    if (Serial.available()>0) {
      return;
    }
    }
    
 
}

void rainbowCycle(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256; j++) { // 1 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
    if (Serial.available()>0) {
      return;
    }
  }
}

void rainbowCycleslow(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256*3; j++) { // 3 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
void rainbowHold(uint8_t wait) {
  uint16_t r, j;

  for(j=0; j<256*1; j++) { // 3 cycles of all colors on wheel
    for(r=0; r< pixels.numPixels(); r++) {
      pixels.setPixelColor(r, Wheel(((r * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
