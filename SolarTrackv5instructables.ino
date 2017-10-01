// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <Wire.h>
#include <Adafruit_INA219.h>


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "esp8266-webupdate";
const char* ssid = "";
const char* password = "";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

Adafruit_INA219 ina219;
Adafruit_INA219 ina219_B(0x41);
Adafruit_INA219 ina219_C(0x45);

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the 'counter' feed
//AdafruitIO_Feed *counter = io.feed("counter");

AdafruitIO_Feed *SolarAmps = io.feed("SolarAmps");
AdafruitIO_Feed *SolarVolts = io.feed("SolarVolts");
AdafruitIO_Feed *SolarWatts = io.feed("SolarWatts");

AdafruitIO_Feed *BatteryAmps = io.feed("BatteryAmps");
AdafruitIO_Feed *BatteryVolts = io.feed("BatteryVolts");
AdafruitIO_Feed *BatteryWatts = io.feed("BatteryWatts");

AdafruitIO_Feed *LoadAmps = io.feed("LoadAmps");
AdafruitIO_Feed *LoadVolts = io.feed("LoadVolts");
AdafruitIO_Feed *LoadWatts = io.feed("LoadWatts");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

 while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  uint32_t currentFrequency;
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  ina219_B.begin();
    ina219_C.begin();

  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
    ina219.setCalibration_32V_32A();
        ina219_B.setCalibration_32V_32A();
            ina219_C.setCalibration_32V_32A();
  //ina219.setCalibration_32V_7A();
  
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");

   while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop() {
  httpServer.handleClient();

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();


//   EVERY_N_SECONDS(10) {
    getSolarValues();
 //  }

delay(10000);
}

void getSolarValues(){
float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
    float current_A = 0;
  float loadvoltage = 0;
    float watts = 0;


    float bshuntvoltage = 0;
  float bbusvoltage = 0;
  float bcurrent_mA = 0;
    float bcurrent_A = 0;
  float bloadvoltage = 0;
    float bwatts = 0;


float lshuntvoltage = 0;
  float lbusvoltage = 0;
  float lcurrent_mA = 0;
    float lcurrent_A = 0;
  float lloadvoltage = 0;
    float lwatts = 0;


  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

current_A = current_mA/1000;
  watts = busvoltage*current_A;
  
    bshuntvoltage = ina219_B.getShuntVoltage_mV();
  bbusvoltage = ina219_B.getBusVoltage_V();
  bcurrent_mA = ina219_B.getCurrent_mA();
  bloadvoltage = bbusvoltage + (bshuntvoltage / 1000);

  bcurrent_A = bcurrent_mA/1000;
  bwatts = bbusvoltage*bcurrent_A;

    lshuntvoltage = ina219_C.getShuntVoltage_mV();
  lbusvoltage = ina219_C.getBusVoltage_V();
  lcurrent_mA = ina219_C.getCurrent_mA();
  lloadvoltage = lbusvoltage + (lshuntvoltage / 1000);

  lcurrent_A = lcurrent_mA/1000;
  lwatts = lbusvoltage*lcurrent_A;

 Serial.println("Panel");
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current mA:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Current A:       "); Serial.print(current_A); Serial.println(" A");
    Serial.print("Watts:       "); Serial.print(watts); Serial.println(" w");
  Serial.println("");
  Serial.println("");

   Serial.println("Battery");
  Serial.print("Bus Voltage:   "); Serial.print(bbusvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(bshuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(bloadvoltage); Serial.println(" V");
  Serial.print("Current mA:       "); Serial.print(bcurrent_mA); Serial.println(" mA");
  Serial.print("Current A:       "); Serial.print(bcurrent_A); Serial.println(" A");
    Serial.print("Watts:       "); Serial.print(bwatts); Serial.println(" w");
  Serial.println("");
  Serial.println("");

   Serial.println("Load");
  Serial.print("Bus Voltage:   "); Serial.print(lbusvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(lshuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(lloadvoltage); Serial.println(" V");
  Serial.print("Current mA:       "); Serial.print(lcurrent_mA); Serial.println(" mA");
  Serial.print("Current A:       "); Serial.print(lcurrent_A); Serial.println(" A");
    Serial.print("Watts:       "); Serial.print(lwatts); Serial.println(" w");
  Serial.println("");
    Serial.println("");
    
 SolarAmps->save(current_A); 
 SolarVolts->save(busvoltage);
 SolarWatts->save(watts);

 BatteryAmps->save(bcurrent_A); 
 BatteryVolts->save(bbusvoltage);
 BatteryWatts->save(bwatts);

 LoadAmps->save(lcurrent_A); 
 LoadVolts->save(lbusvoltage);
 LoadWatts->save(lwatts);


}//end of get solar


