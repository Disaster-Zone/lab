/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SI1145.h>
#include <Adafruit_TSL2561_U.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bmp; // I2C
Adafruit_SI1145 uv = Adafruit_SI1145();
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

ESP8266WiFiMulti WiFiMulti;

char payload[200];

void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  if (!uv.begin()) {
    Serial.println(F("Could not find a valid SI1145 sensor, check wiring!"));
    while (1);
  }

  if (!tsl.begin()) {
    Serial.println(F("Could not find a valid TSL2561 sensor, check wiring!"));
    while (1);
  }

  configureTSL2561();
  
  WiFiMulti.addAP("Network_Name", "Network_Password");   // Don't check into Github your network info!
}

void loop() {

  // wait for wifi connection
  if (WiFiMulti.run() == WL_CONNECTED) {
    
    HTTPClient http;
    http.begin("http://192.168.187.142:4000/api/weather/entries");
    http.addHeader("content-type", "application/json");

    float tempFloat = bmp.readTemperature();
    char  temp_str[6];
    dtostrf(tempFloat, 4, 2, temp_str);

    float baroFloat = bmp.readPressure();
    char  baro_str[10];
    dtostrf(baroFloat, 6, 0, baro_str);

    float altFloat = bmp.readAltitude(1013.25);   // this should be adjusted to your local forcase
    char  alt_str[10];
    dtostrf(altFloat, 6, 2, alt_str);

    float uvFloat = uv.readUV() / 100.0;
    char  uv_str[10];
    dtostrf(uvFloat, 6, 2, uv_str);

    /* Lux Sensor logic */
    sensors_event_t event;
    tsl.getEvent(&event);

    float luxFloat = 0.0;
    if (event.light) {
      luxFloat = event.light;
    }
    else {
      Serial.println(F("TSL2561 Sensor overload"));
    }
    char lux_str[10];
    dtostrf(luxFloat, 6, 2, lux_str);
    
    sprintf(payload, "{\"sensor\":{\"wind_speed\":5.5,\"wind_direction\":\"E\",\"uv\":%s,\"temperature\":%s,\"alt\":%s,\"humidity\":19,\"barometric\":%s,\"lux\":%s}}",
            uv_str,
            temp_str,
            alt_str,
            baro_str,
            lux_str);

    int httpCode = http.POST(payload);

    if (httpCode > 0) {
      Serial.print("[HTTP] POST ... Code: ");
      Serial.println(httpCode);

      http.end();
    }
    
    Serial.print(F("Temperature = "));
    Serial.print(temp_str);
    Serial.println(" *C");
      
    Serial.print(F("Pressure = "));
    Serial.print(baro_str);
    Serial.println(" Pa");
  
    Serial.print(F("Approx altitude = "));
    Serial.print(alt_str);
    Serial.println(" m");
      
    Serial.println();
  }
                                // 5m   1m     1s   WS roundtrip
  delay((5 * 60 * 1000) - 45);  //  5 * 60 * 1000 - 45
}


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureTSL2561(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}
