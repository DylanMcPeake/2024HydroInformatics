/*
Assignment 2 - Datalogger Programming and Data Collection
Created By: Dylan McPeake
Creation Date: 1/25/2024
Last Modified: 2/3/2024

SDCard Breakout Wiring:
-----------------------
Red - 5V
Black - GRD
Yellow - SDA
White - SCL
D13 - CL
D12 - DO
D11 - DI
D10 - CS
*/

// Include libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_AHTX0.h>

// Initialize variables & instances
Adafruit_AHTX0 aht;
bool readsensor;
unsigned long scanInterval = 1;
unsigned long recordInterval = 10;
unsigned long currMillis = 0;       
unsigned long prevMillis = 0;
unsigned long prevRecordMillis = 0;
unsigned long scanCounter = 0;
unsigned long recordCounter = 0;  
float randomNum;
float humSum;
float tempSum;
float humAvg; 
float tempAvg;   

void setup() {
  // Set up a Serial port for output
  Serial.begin(9600);
  Serial.println("Initializing the SD card...");
  
  // Initialize the SD Card & Connection Check
  pinMode(10, OUTPUT);
  if (! SD.begin(10)){
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialization done.");

  // Initialize communications with the sensor
  if (! aht.begin()) {
    Serial.println("Could not find sensor. Check wiring!");
    while (1) {
      delay(10);
    } 
  }
  Serial.println("Sensor found");

  // Create a new file on the SD
  File myFile = SD.open("datalog.txt", FILE_WRITE);

  // Metadata
  myFile.println("Dylan McPeake - CEE 6610 - Assignemnt 2");
  Serial.println("Dylan McPeake - CEE 6610 - Assignemnt 2");
  myFile.println("Temperature and Relative Humidity Sensor Output");
  Serial.println("Temperature and Relative Humidity Sensor Output");
  myFile.println("Measurements using a AM2301B Sensor");
  Serial.println("Measurements using a AM2301B Sensor");
  myFile.println("Recorded Personal PC Temperature from  Feb 3 8:00pm to Feb 4 8:00pm");
  Serial.println("Recorded Personal PC Temperature from  Feb 3 8:00pm to Feb 4 8:00pm");
  myFile.println("Time (s), Relative Humidity (%), Temperature (Deg C)");
  Serial.println("Time (s), Relative Humidity (%), Temperature (Deg C)");
  
  // Close the file 
  myFile.close();
}

void loop() {
  // Open the file again
  //File myFile = SD.open("text.txt", FILE_WRITE);
  currMillis = millis();

  // Create the hum and temp sensor event objects that will hold the measurement results
  sensors_event_t hum; 
  sensors_event_t temp;
  
   if ((currMillis - prevMillis) >= (scanInterval * 1000)){
    //Take data every one second (scannging)
    scanCounter ++;
    readsensor = aht.getEvent(&hum, &temp); 
    humSum += hum.relative_humidity;
    tempSum += temp.temperature;
    prevMillis = currMillis;

    if ((currMillis - prevRecordMillis) >= (recordInterval * 1000)){
      // Average scanned values together to 
      humAvg = humSum / scanCounter; 
      tempAvg= tempSum / scanCounter;
      recordCounter ++;
      // Divide currMillis by 1000 to convert to seconds
      String dataRecord = String((float)currMillis/1000) + ", " + String(humAvg) + ", " + String(tempAvg);
      File myFile = SD.open("datalog.txt", FILE_WRITE);
      myFile.println(dataRecord);
      Serial.println(dataRecord);
      myFile.close();
      prevRecordMillis = currMillis;
      scanCounter = 0;
      humSum = 0;
      tempSum = 0;
    }
  }
}
