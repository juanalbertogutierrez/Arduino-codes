
int IN3 = 5;                    // Declaracion de uso del pin 5 como entrada
int IN4 = 4;                    // Declaracion de uso del pin 4 como entrada
int ENB = 3;                    // Declaracion de uso del pin 3 como ENB

void setup()
{
 pinMode (ENB, OUTPUT);         //Pin en salida
 pinMode (IN3, OUTPUT);         //Pin en salida
 pinMode (IN4, OUTPUT);         //Pin en salida
}


void loop()
{
  digitalWrite (IN3, HIGH);     //El motor gira en sentido a las manecillas del reloj pin3 alto pin 4 bajo
  digitalWrite (IN4, LOW);
  analogWrite(ENB,180);         //Potencia del motor a 180
  delay(2000);                  //Tiempo de espera 2000 milisegundos
  analogWrite(ENB,185);
  delay(2000);
  analogWrite(ENB,180);
  delay(2000);
  analogWrite(ENB,0);          //Potencia del motor a 0
  delay(5000);
  digitalWrite (IN3, LOW);     //El motor gira en sentido contrario a las manecillas del reloj pin 3 bajo pin 4 alto 
  digitalWrite (IN4, HIGH);
  analogWrite(ENB,190);
  delay(2000);
  analogWrite(ENB,185);
  delay(2000);
  analogWrite(ENB,180);
  delay(2000);
  analogWrite(ENB,0);
  delay(5000);
}
