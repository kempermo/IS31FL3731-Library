#include<IS31FL3731.h>

IS31FL3731 ledDriver = IS31FL3731();
int lastLed = 1;
int nextLed = 0;
String inputString = "";

void setup()
{
  Serial.begin(9600);

  if (!ledDriver.begin())
  {
    Serial.println("IS31 not available");
    while (1);
  }

  Serial.println("IS31 ready!");
}

void loop()
{
//Uncomment the following to fill the whole matrix
//    for(int y =0; y < 11; y++)
//    {
//      for (int x = 0; x < 11; x++)
//      {
//        ledDriver.drawPixel(x, y, 255);
//        delay(100);
//      }
//    }

//Uncomment teh following to show the Pixels through Serial command one by one
//  ledDriver.setLEDPWM(lastLed, 0, 0);
//  ledDriver.setLEDPWM(nextLed, 255, 0);
//  //displayNumber(nextLed);
//  delay(100);
//
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    if (inChar == '\n') {
//      lastLed = nextLed;
//      nextLed = inputString.toInt();
//      //displayNumber(nextLed);
//      inputString = "";
//      Serial.println(nextLed);
//    }
//  }
}
