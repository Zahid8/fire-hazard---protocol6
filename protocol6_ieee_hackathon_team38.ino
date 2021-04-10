//Team name : Protocol6
//Team Leader : B Zahid Hussain
//problem statement number : 9
//Team number : 38

#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial wifi_module(2,3); 
int RLED = 9;
int GLED = 8;
int buzzer = 10;
int gas_sen = A0;

void setup()
{
  Serial.begin(9600);
  wifi_module.begin(9600); 
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(gas_sen, INPUT);
  esp8266_command("AT+RST\r\n",2000,DEBUG); 
  esp8266_command("AT+CWMODE=2\r\n",1000,DEBUG); 
  esp8266_command("AT+CIFSR\r\n",1000,DEBUG); 
  esp8266_command("AT+CIPMUX=1\r\n",1000,DEBUG); 
  esp8266_command("AT+CIPSERVER=1,80\r\n",1000,DEBUG); 
}

void loop()
{
  int analogSensor = analogRead(gas_sen);
  if (analogSensor > 350)
  {
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, LOW);
    tone(buzzer, 1000, 200);
  }
  else
  {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, HIGH);
    noTone(buzzer);
  }
  if(wifi_module.available()) 
  {
    if(wifi_module.find("+IPD,"))
    {
     delay(1000);
     int connectionId = wifi_module.read()-48;     
     String webpage = "<h1>IOT Smoke Detection System</h1>";
      webpage +="<p>Smoke Value is ";
      webpage += analogSensor;
      webpage +="</p>";
      if (analogSensor > 400)
  {
    webpage +="<h5>DANGER! Move Somewhere Else</h5>";
  }
  else
  {
    webpage +="<h4>Everything Normal</h4>";
  }
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";     
     esp8266_command(cipSend,1000,DEBUG);
     esp8266_command(webpage,1000,DEBUG);     
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; 
     closeCommand+="\r\n";     
     esp8266_command(closeCommand,3000,DEBUG);
    }
  }
}
String esp8266_command(String command, const int timeout, boolean debug)
{
    String response = "";
    wifi_module.print(command); 
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(wifi_module.available())
      {     
        char c = wifi_module.read(); 
        response+=c;
      }  
    }
    if(debug)
    {
      Serial.print(response);
    }
    return response;
}