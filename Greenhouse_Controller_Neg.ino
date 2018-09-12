/*Project_Weather_Station_Controller
Program has the following function:
Reads a DHT11 Temperature and Humidity sensor on Digital Pin 2,
Display the values on a OLED Display throught the I2C bus. 
Relay attached to Digital pin 3.
Any temperature reading over 82 degress will enable a negative output on digital pin 3 that will engage the realy.
The Relay will enable the AC power to engage fan motor.  
Written by Rusty Cain Jan 27 2016 Updated Sept 12, 2018
This program is just a simple example and is not a perfect example of how to do this.
Please use and modify and let me know of any improvements*/

#include <Wire.h>                            // Enables I2C for OLED 
#include "DHT.h"                             // DHT library
#include "U8glib.h"                          // OLED Library
#define DHTTYPE DHT11                        // DHT 11
#define DHTPIN 2                             // what digital pin we're connected to
DHT dht(DHTPIN, DHTTYPE);
//U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);       // D0=13, D1=11, CS=10, DC=9, Reset=8
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);       // I2C / TWI 
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST); // Dev 0, Fast I2C / TWI

float t;                                    // Variable for temperature 
float h;                                    // Variable for Humidity 
int FanOn = 1;                              // 0 = FanOn - 100 = Fan Off

void draw(void) {
    u8g.setFont(u8g_font_unifont);          // select font
    u8g.drawStr(0, 10, " DMS Workshop ");   // put string of display at position X, Y
    u8g.drawStr(0, 30, "Temp: ");           // put string of display at position X, Y
    u8g.drawStr(0, 50, "Hum: ");            // put string of display at position X, Y
    u8g.setPrintPos(44, 30);                // set position
    u8g.print(t, 0);                        // display temperature from DHT11
    u8g.drawStr(67, 30, "F ");              // put string of display at position X, Y
    u8g.setPrintPos(44, 50);                // put string of display at position X, Y
    u8g.print(h, 0);                        // display humidity from DHT11
    u8g.drawStr(67, 50, "% ");              // put string of display at position X, Y
}

void setup(void) {
    pinMode(3, OUTPUT);                     // Set up FanOn Pin
    Serial.begin(9600);                     // Start commuication with serial console   
    FanOn = 0;                              // Fan Status 100 = On - 0 = Off
    Serial.print("Startup Please wait ");
    delay (2000);
   }

void loop(void)  {
    FanOn = 0;                              // Fan Status = Off
    Serial.print("FanOn 1 ");               // Send Status of Fan to console port 
    Serial.println(FanOn);                  // Send Status of Fan to console port 
    Serial.println("DMS Sept 2018 Workshop DHTxx & OLED ");
    Serial.println("Greenhouse_Controller_Neg ");    
    u8g.firstPage();  
  do 
  {
     t = read_temp();                        // Read Temperature Sensor  
      if ( t > 82.00 )                       // Check temperature value if over the limit turn on Fan
      {
        FanOn = 100;                         // Set Fan Status = On. Use 100 to enable more than one Fan or device to be activated
        TurnFanOnFunction();                 // Call Function to turn on Fan
      }
    if ( t < 82.00 )                         // Check temperature value if under the limit turn Fan off 
      {
        FanOn = 0;                           // Set Fan Status = Off
        TurnFanOnFunction();                 // Call Function to turn off Fan
      }
    h = read_hum();                          // Read Humidity Sensor   
    draw();                                  // Call Function to write to Display
  }   
while( u8g.nextPage() );
    delay(3000);                             // Delay of 3 sec before accessing DHT11 (min - 2sec)
  }

float read_temp() {
    float temp = dht.readTemperature(DHTPIN);// Read apin on DHT11
    Serial.println(temp);                    // Send Value to console port 
    return temp;                             // Return Value for evaluation  
  }
float read_hum() {
     float hum = dht.readHumidity(DHTPIN);   // Read apin on DHT11
     Serial.println(hum);                    // Send Value to console port 
     return hum;                             // Return Value for evaluation
  }
int TurnFanOnFunction()
{
  if( FanOn == 0 ) 
  {
   digitalWrite(3, HIGH);                     // Turn Fan On
   Serial.print("DHTxx Fan Off! ");          // Send Status of Fan to console port 
   Serial.println(FanOn);                    // Send Status of Fan to console port 
   FanOn = 0;
  }
  
  if( FanOn == 100 )                        // Use 100 to enable more than one Fan or device to be activated
  {
    digitalWrite(3, LOW);                   // Turn Fan On
    Serial.print("DHTxx Fan On! ");          // Send Status of Fan to console port 
    Serial.println(FanOn);                   // Send Status of Fan to console port 
    delay (1);
  }
    return (0); 
}
