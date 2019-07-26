#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+90xxxxxxxxxx"; //-> change with your number

#include <dht.h>
#include <DHT.h>
#include "Arduino.h"
#define dht_apin A0
const int sensor=A0;
int periyot=20;
double alfa=0.75;
double yenileme=0.0;
dht DHT;
 
void setup(){
 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("SAMED DEMİR && MAHİRCAN YÜKSEL\n\n");
  delay(1000);//Wait before accessing Sensor
  pinMode(sensor,INPUT);
  delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
  
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
//    DHT.read11(dht_apin);
//    
//    Serial.print("Nem Oranı = ");
//    Serial.print(DHT.humidity);
//    Serial.print("%  ");
//    Serial.print("Sıcaklık = ");
//    Serial.print(DHT.temperature); 
//    Serial.println("C  ");
//    
//    delay(5000);//Wait 5 seconds before accessing sensor again.
//
//    static double oncekideger=0;
//    static double oncekiyenileme=0;
//
//    int ritim=analogRead(sensor);
//
//    double deger=alfa*oncekideger+(0-alfa)*ritim;
//    yenileme=deger-oncekideger;
//    Serial.print("Nabız = ");
//    Serial.println(ritim/10);
//    oncekideger=deger;
//    oncekiyenileme=yenileme;
//    delay(periyot*10);
    
     if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
}
void SendMessage()
{
  DHT.read11(dht_apin);
    
//    Serial.print("Nem Oranı = ");
//    Serial.print(DHT.humidity);
//    Serial.print("%  ");
//    Serial.print("Sıcaklık = ");
//    Serial.print(DHT.temperature); 
//    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.

    static double oncekideger=0;
    static double oncekiyenileme=0;

    int ritim=analogRead(sensor);

    double deger=alfa*oncekideger+(0-alfa)*ritim;
    yenileme=deger-oncekideger;
//    Serial.print("Nabız = ");
//    Serial.println(ritim/10);
    oncekideger=deger;
    oncekiyenileme=yenileme;
    delay(periyot*10);
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  int SMS = ritim/10+"ve"DHT.humidity+"ve"+DHT.temperature;
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  delay (1000);
  sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write ("Unread Message done");
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}
    
 
 
