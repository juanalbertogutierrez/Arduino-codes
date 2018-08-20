
#define Pecho 6
#define Ptrig 7
long duracion, distancia;

void setup() {
  Serial.begin(9600);
  pinMode(Pecho, INPUT);
  pinMode(Ptrig, OUTPUT);
  pinMode(13,1);

}

void loop() {
  digitalWrite(Ptrig,LOW);
  delay(2000);
  digitalWrite(Ptrig,HIGH);
  delay(2000);
  digitalWrite(Ptrig,LOW);

  duracion = pulseIn(Pecho,HIGH);
  distancia=(duracion/2)/29;
  if(distancia>=500 || distancia <= 0){
    Serial.print("---");
  }
  else{
    Serial.print(distancia);
    Serial.println("cm");
    digitalWrite(13,0);
  }

  if(distancia <=10 && distancia >=1){
    digitalWrite(13,1);
    Serial.println("Alarma.....");
  }
  delay(400);
}
