#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define BLYNK_AUTH_TOKEN "bowGAm2MHVEHGSeiQx38lkW9amDCkEtv" //Enter your blynk auth token

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SLT-4G_1694C2";//Enter your WIFI name
char pass[] = "NavodKeepYourMind2021!";//Enter your WIFI password


BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 1) {
    digitalWrite(D0, LOW);
  } else {
    digitalWrite(D0, HIGH);
  }
}


boolean startup = true;
int gasMeasure = 0;
int gasMeasureLimit = 300;
long PIRState = LOW;
int PIRDelay = 300000;

void morningSession(){
    startup = false;
    Serial.println("Day-time");
    digitalWrite(D3,LOW);
    delay(2000);
    //digitalWrite(D0,HIGH);
    delay(1000);
    gasMeasure = analogRead(A0);

    if(gasMeasure > 300 && gasMeasure < 350){
      for(int i = 0; i < 3; i++){
        digitalWrite(D3,HIGH);
        delay(500);
        digitalWrite(D3,LOW);
        delay(500);
      }
    }else if(gasMeasure > 350){
        Serial.print("GAS Leak : ");
        Serial.println(gasMeasure);
        Serial.println("");
        digitalWrite(D6,LOW);
        digitalWrite(D7,HIGH);
        for(int i = 0; i < 10; i++){
          digitalWrite(D3,HIGH);
          delay(500);
          digitalWrite(D3,LOW);
          delay(500);
        }
        
      }else{
        Serial.print("No GAS Leak : ");
        Serial.println(gasMeasure);
        Serial.println("");
        digitalWrite(D6,HIGH);
        digitalWrite(D7,LOW);
      }
  }


void nightSession(){
  while(1){
    Serial.println("Night- day");
    digitalWrite(D3,LOW);
    PIRState = digitalRead(D8);
    if(PIRState == HIGH){
      Serial.println("Motion detected!");
      digitalWrite(D0,LOW);
      delay(PIRDelay);
      digitalWrite(D0,HIGH);
      continue;
    }
    else{
      Serial.println("No more motion");
      //digitalWrite(D0,HIGH);
      delay(100);
    }
    gasMeasure = analogRead(A0);

    if(gasMeasure > 300 && gasMeasure < 350){
      for(int i = 0; i < 3; i++){
        digitalWrite(D3,HIGH);
        delay(500);
        digitalWrite(D3,LOW);
        delay(500);
        }
      }else if(gasMeasure > 350){
        Serial.print("GAS Leak : ");
        Serial.println(gasMeasure);
        Serial.println("");
        digitalWrite(D6,LOW);
        digitalWrite(D7,HIGH);
          for(int i = 0; i < 10; i++){
            digitalWrite(D3,HIGH);
            delay(500);
            digitalWrite(D3,LOW);
            delay(500);
          }
        
      }else{
        Serial.print("No GAS Leak : ");
        Serial.println(gasMeasure);
        Serial.println("");
        digitalWrite(D6,HIGH);
        digitalWrite(D7,LOW);
      }
      return;
  }
}

void setup() {

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  pinMode(D0,OUTPUT); // Relay
  pinMode(D2,OUTPUT); // GAS Sensor - DO
  pinMode(D3,OUTPUT); // Buzer
  pinMode(D6,OUTPUT); // LED - Green
  pinMode(D7,OUTPUT); // LED - Red
  pinMode(D8,INPUT); // PIR Sensor
  pinMode(A0,INPUT);  // GAS Sensor - AO

  Serial.begin(9600);
 
}

void loop() {

  Blynk.run();
  
  if(startup == true){
    digitalWrite(D3,HIGH);
    delay(1000);
    digitalWrite(D0,LOW);
    delay(3000);
    digitalWrite(D3,HIGH);
    digitalWrite(D0,HIGH);
    startup = false;
  }
  else{
    if(digitalRead(D1)==LOW){
      // Day Function
      morningSession();
    }
   if(digitalRead(D1)==HIGH){
      // Night Function
      nightSession();
    }
  }  
}
