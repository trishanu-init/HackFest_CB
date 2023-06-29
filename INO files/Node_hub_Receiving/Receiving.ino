#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial HC12(12, 13);//tx,rx

int hum=0;
int temp=0;
int flame=0;
int cogas=0;
String input;
int boundLow;
int boundHigh;
const char delimiter = ',';


void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
}


void loop() {
  if(HC12.available())
  {
  input = HC12.readStringUntil('\n');
  if (input.length() > 0)
      {
        Serial.println(input);
       
       boundLow = input.indexOf(delimiter);
        hum = input.substring(0, boundLow).toInt();
        Serial.println(hum);
        boundHigh = input.indexOf(delimiter, boundLow+1);
        temp = input.substring(boundLow+1, boundHigh).toInt();
        Serial.println(hum);
        boundLow = input.indexOf(delimiter);//you can add 2 variables, or 4... by adding these 4 lines 
        flame = input.substring(0, boundLow).toInt();//you can add 2 variables, or 4... by adding these 4 lines
        Serial.println(hum);
        boundHigh = input.indexOf(delimiter, boundLow+1);//you can add 2 variables, or 4... by adding these 4 lines 
        cogas = input.substring(boundLow+1, boundHigh).toInt();//     under this line
//but before this one
//if you change the number of variables, follow instructions in comments above
//duplicating the lines as explained adds 2 variables, then you can duplicate them again, and then change the variable names so they are in the right order
  
delay(10);
      }//after this you can choose what to do with each variable, which will now have the same value as it's value on the transmitter end
  }
}
