// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include<SoftwareSerial.h>
SoftwareSerial bt(3,4); /* (Rx,Tx) */
long instance1=0, timer, lasttimer;

double hrv =0, hr = 72, interval = 0;

int value = 0, count = 0;  

bool flag = 0;


#define threshold 434 // to identify R peak

#define timer_value 10000 // 10 seconds timer to calculate hr

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  bt.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
 
  // Start up the library
  sensors.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
    instance1=micros();
    timer=millis();
    value = analogRead(A0);

    value = map(value, 250, 400, 0, 100); //to flatten the ecg values a bit
    
    if((value >= threshold) ) {

      count++;  
      Serial.print("count=");
      Serial.println(count);

      flag = 1;

      interval = micros() - instance1; //RR interval

      instance1 = micros(); 

    }

    else if((value < threshold)) {

      flag = 0;

    }
   Serial.println(timer-lasttimer);
   if (timer-lasttimer >20000) {
      Serial.println(count);
      hr = count*6;
      //Serial.println(hr);
      lasttimer = millis();
      count=0;
    }
    //hr=count*6;
    hrv = hr/60 - interval/1000000;

    
  // print out the value you read:
  //1Serial.println(sensorValue);
  // about 256Hz sample rate
  delayMicroseconds(3900);
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    //Serial.print("Temperature for the device 1 (index 0) is: ");
    //Serial.println(tempC);
  } 
  else
  {
    //Serial.println("Error: Could not read temperature data");
  }
    Serial.print(hr);

    Serial.print(",");
    Serial.print(hrv);
    Serial.print(",");
    Serial.print(tempC);
    Serial.print(",");
    Serial.println(value);
    bt.print(value);
    bt.print(",");
    bt.println(tempC);
    if(tempC>36)
    bt.println("Temperature Elevated");
    if(hrv>1.40 && hrv <1.8 )
    {
      Serial.println("RR Interval Elevated");
      bt.println("RR Interval Elevated");
    }
    if(hrv>1.8)
    {
    Serial.println("ST Segment Elevated");
    bt.println("ST Segment Elevated");
    }

} 
