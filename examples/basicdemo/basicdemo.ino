#include<IS31FL3731.h>

IS31FL3731 ledDriver = IS31FL3731();

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
  for (int x = 0; x < 11; x++)
  {
    ledDriver.drawPixel(x, 0, 255);
  }
}
