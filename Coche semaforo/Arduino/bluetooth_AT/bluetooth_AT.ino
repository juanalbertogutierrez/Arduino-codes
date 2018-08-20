#include <SoftwareSerial.h> //Librería puertos seriales

const int LED =13;
const int BTPWR =12;

String nombreBT="AutoNOMO";
String pin="0000";
char velocidad='4';
char modo='0';
 
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BTPWR, OUTPUT);
  
  Serial.begin(38400);
  digitalWrite(LED, HIGH);
  delay(4000);
  digitalWrite(LED, LOW);
  digitalWrite(BTPWR, HIGH);
  delay(10000);
  Serial.print("AT\r\n");
  Serial.print("AT+NAME:"+nombreBT+"\r\n");
  Serial.print("AT+PIN:"+pin+"\r\n");
  Serial.print("AT+BAUD:4\r\n");
  Serial.print("AT+MODE:0\r\n");
  digitalWrite(LED, HIGH);
  }
 
void loop()
{
}
