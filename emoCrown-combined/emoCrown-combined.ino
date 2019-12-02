
#include <Brain.h>

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.

#include <PulseSensorPlayground.h>
#include <Adafruit_NeoPixel.h>
#include<Wire.h>
#include <Brain.h>

Brain brain(Serial);
const int MPU = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
#define PIN 8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(40, PIN);

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

uint8_t  // Current animation effect
offset = 0;
uint32_t color  = 0XFFFFFF; // Starting color
uint32_t prevTime;

void setup() {
    // Start the hardware serial.
    Serial.begin(9600);
    Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
  pixels.begin();
  pixels.setBrightness(40); // 1/3 brightness
  prevTime = millis();
  for (int r = 0; r <= 6; r++) {
    pixels.setPixelColor(r, pixels.Color(50, 50, 50));
  }
  pixels.show();
}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"    
    if (brain.update()) {
        //Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
    }
//     delay(300);
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

//delay(1000);

for (int r = 0; r <= 6; r++) {
    pixels.setPixelColor(r, pixels.Color(AcX/60, AcY/120, AcZ/80));
  }
  pixels.show();


int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
//Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
Serial.print("BPM: ");                        // Print phrase "BPM: " 
Serial.println(myBPM);                        // Print the value inside of myBPM. 
}

//delay(300);
}
