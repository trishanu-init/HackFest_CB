#include <Arduino.h>
#include <SoftwareSerial.h>
//#include <WiFi.h>               //we are using the ESP32
#include <ESP8266WiFi.h>      // uncomment this line if you are using esp8266 and comment the line above
#include <Firebase_ESP_Client.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


#define Flamepin D2
#define DHT11_PIN D7

// Insert your network credentials
#define WIFI_SSID "Thug Life"
#define WIFI_PASSWORD "apnakharidbsdk"

#define BOT_TOKEN "6196315350:AAHB6iBqEcJZJdk2bCyow-xl_aC4MJlwLC8"

#define CHAT_ID "-1001801763261"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBTTaJu-T5D84-Qcj8T6zkZSUnYIcJL9O4"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://hackfest-23-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;



unsigned long sendDataPrevMillis = 0;
int hum;//variables that will get sent
int temp;
int cogas;
int flame;
float latitude , longitude;
const int AOUTpin=A0;//the AOUT pin of the CO sensor goes into analog pin A0 of the 
bool signupOK = false;                     //since we are doing an anonymous sign in 

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup(){
  Serial.begin(115200);
  pinMode(Flamepin, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

Serial.print("Retrieving time: ");
configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
time_t now = time(nullptr);
while (now < 24 * 3600)
{
  Serial.print(".");
  delay(100);
  now = time(nullptr);
}
Serial.println(now);
bot.sendMessage(CHAT_ID, "Forest Fire detection activated\nSector : 87J3", "");
}

void loop(){
  delay(3000);
  DHT.read(DHT11_PIN);
  int hum = DHT.humidity;
  int temp = DHT.temperature;
  int flame = digitalRead(Flamepin);
  
  cogas= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin

  Serial.print(hum);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(flame); 
  Serial.print(","); 
  Serial.print(cogas);
  Serial.print("\n");


  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/hum", hum)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
       
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setInt(&fbdo, "test/temp",temp)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());}

    if (Firebase.RTDB.setInt(&fbdo, "test/cogas",cogas)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());}

    if (Firebase.RTDB.setInt(&fbdo, "test/flame",flame)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());}
}
  if(flame==0){
    bot.sendMessage(CHAT_ID, "🔥POTENTIAL FOREST FIRE DETECTED !!🔥\nSector : 87J3", "");
  }
  

}