#include <WiFiNINA.h>
#include "secrets.h"

// Light Sensor Setup.
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

// Light Intensity.
float LightIntensity = 0;
bool Sunlight = false;

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME_1 = "/trigger/SunlightStarted/with/key/fKBhj6gr9gWgVeYX5AiB_"; // change your EVENT-NAME and YOUR-KEY
String PATH_NAME_2 = "/trigger/SunlightEnded/with/key/fKBhj6gr9gWgVeYX5AiB_";
String queryString = "?value1=57&value2=25";

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  Wire.begin();
  while (!Serial);

  /*// connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }*/

  // Initialise Light Sensor.
  lightMeter.begin();
}

void loop()
{
  // Collect light data from sensor.
  LightIntensity = lightMeter.readLightLevel();

  if (LightIntensity > 100 & Sunlight == false)
  {
    Sunlight = true;
    Serial.println("True");
    if (client.connect(HOST_NAME, 80))
    {
      // if connected:
      Serial.println("Connected to server");
    }
    else 
    {
      // if not connected:
      Serial.println("connection failed");
    }

    // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME_1 + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected())
    {
      if (client.available())
      {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
  else if (LightIntensity <= 100 & Sunlight == true)
  {
    Sunlight = false;
    Serial.println("False");
    // connect to web server on port 80:
    if (client.connect(HOST_NAME, 80))
    {
      // if connected:
      Serial.println("Connected to server");
    }
    else 
    {
      // if not connected:
      Serial.println("connection failed");
    }

    // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME_2 + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected())
    {
      if (client.available())
      {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }

  Serial.print("Light Intensity: ");
  Serial.print(LightIntensity);
  Serial.println(" lx");
  delay(1000);
}