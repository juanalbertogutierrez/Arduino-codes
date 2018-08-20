/*
Autor: Juan Alberto Gutierrez Canto
Descripcion: utilizacion de arduino y servomotor, mueve el arduino
a=10 grados mas, z= 10 grados menos.
*/

#include <Servo.h>

Servo miServo;
int angulo= 90;

void setup(){
  miServo.attach(9); //indicamos en que pin trabajara
  Serial.begin(9600); //utilizaremos la consola serial
}

void loop() {
  unsigned char comando=0;
  if(Serial.available()){
    comando=Serial.read();//leemos de pantalla
    (comando=='a')?angulo+=90:angulo-=90;
    angulo= constrain(angulo,0,180);
  }
  miServo.write(angulo);
  Serial.print("angulo: ");
  Serial.println(angulo);
  delay(2000);
}
