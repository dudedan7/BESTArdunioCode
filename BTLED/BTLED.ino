void setup() {
Serial.begin(9600);
 // put your setup code here, to run once:
 pinMode(8, OUTPUT);
 }

void loop() {
  // put your main code here, to run repeatedly:
 if(Serial.available()>0)
   {     
      char data= Serial.read(); // reading the data received from the bluetooth module
      switch(data)
      {
        case 'y': digitalWrite(8, HIGH);break; // when a is pressed on the app on your smart phone
        case 'n': digitalWrite(8, LOW);break; // when d is pressed on the app on your smart phone
        default : break;
      }
      Serial.println(data);
   }
   delay(50);
}
