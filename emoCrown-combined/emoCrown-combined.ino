// BLE + JEWEL
#include <Adafruit_NeoPixel.h>
#define PIN 5
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, PIN);
uint8_t  mode   = 0, // Current animation effect
         offset = 0;
uint32_t color  = 0XFFFFFF; // Starting color
uint32_t prevTime; 

//IMU
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//EEG
#include <Brain.h>
Brain brain(Serial);

//GESTURE
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#define APDS9960_INT    2 // Needs to be an interrupt pin
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;


void setup() {

  // BLE + JEWEL
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(40); // 1/3 brightness
  prevTime = millis();

// IMU
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

// GESTURE
// Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);
  // Initialize Serial port
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  
}

void loop() {
//EEG
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"    
    if (brain.update()) {
        //Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
    }
  
// BLE + JEWEL
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

//IMU
Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read(); 
Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(1000);

//GESTURE
if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  
}

//GESTURE
void interruptRoutine() {
  isr_flag = 1;
}

//GESTURE
void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}

// BLE + JEWEL
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

// BLE + JEWEL
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

// BLE + JEWEL
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

// BLE + JEWEL
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
// BLE + JEWEL
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
// BLE + JEWEL
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
