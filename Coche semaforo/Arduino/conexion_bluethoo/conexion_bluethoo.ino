//bluetooth hc-06
int ledPin = 13; // usamos un pin de salida al LED
char state; // Variable lectrura serial
String cadena="";
 
void setup() {
    pinMode(ledPin, OUTPUT);   //Declara pin de Salida
    digitalWrite(ledPin, LOW); //Normalmente Apagado
    Serial.begin(9600);
}
 
void loop() {
  int cont=0;
  if(Serial.available()){
    cadena="";
      do{
       state = Serial.read();
       cadena+=state;
       cont++;
      }while(state!=',');
  }
   for(int i=0;i<cont;i++){
    Serial.println(cadena[i]);
   }
 
 delay(10000);
 // si el estado es 0 ese sería Apagado “OFF”
 if (state == '0') {
    digitalWrite(ledPin, LOW);
    Serial.println("LED: off");
 }
 
 // de lo contrario si el estado es 1 ese sería Encendido “ON”
 else
 if (state == '1') {
     digitalWrite(ledPin, HIGH);
     Serial.println("LED: on");
 }
 
}
