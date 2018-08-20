/*  Aplicacion arduino. 
 *  Proyecto arduino- Estacionado automatico
 *  Controladores Motor traccion, servo motor y bt
 *  Juan Alberto Gutierrez Canto, Ingeniero en Software.
 *  enero 6 del 2017. version 1.0
*/

#include <mcp_can.h>                          //Biblioteca para la integración del BUS CAN Shield con arduino
#include <SPI.h>                              //Permite al arduino comunicarse con dispositivos SPI (Serial Peripheral Interface)
#include <Servo.h>
#define CAN0_INT 2                            //  definir el valor para pin 2
                                              // the cs pin of the version after v1.1 is default to D9
                                              // v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;                     //Pin que usara el can shield para el envió/recepción de mensajes de la red can
MCP_CAN CAN(SPI_CS_PIN);                      // Configura el pin 9 (SPI_CS_PIN) para la red bus can

int posmotor_adelante=5;                       //Pin para direccion del motor traccion adelante 
int posmotor_atras=4;                         //Pin para direccion del motor traccion atras
int potenciamotor_ENB=3;                      //Pin para potencia del motor traccion
Servo miServo;
int servo_PIN= 7;
int angulo= 90;

void setup() {
  Serial.begin(9600);                         //Inicializa el puerto serial a un baudio de 9600
  START_INIT:                                 //Retorno del goto
  if(CAN_OK == CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)){       // Si el bus can inciia con exito al inicializa el bus can con un baudio de 500K
    Serial.println("CAN BUS Shield init ok!");//Imprime el mensaje al monitor serie de exito
  }
  else{                                       //Si no se pudo inicializar el can bus
    Serial.println("CAN BUS Shield init fail");//Imprime el mensaje al monitor serial de fracaso
    Serial.println("Init CAN BUS Shield again");
    delay(100);                                //Genera un retraso de 100 milisegundos
    goto START_INIT;                           //Regresamos hasta que se inicialice el bus can
  }
  digitalWrite(potenciamotor_ENB, OUTPUT);     //Indicamos que el pin 3 es de salida
  digitalWrite(posmotor_adelante, OUTPUT);     //Indicamos que el pin 5 es de salida
  digitalWrite(posmotor_atras, OUTPUT);     //Indicamos que el pin 4 es de salida
  miServo.attach(servo_PIN);                          //Indicamos que el servo motor trabajara en el pin 7
  pinMode(CAN0_INT,INPUT);                    //entrada de mensaje can
  //CAN.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

unsigned char motor_parado[8] =    {0, 0, 0, 0, 0, 0, 0, 2};     //Mensaje personalizado para indicar que el motor dara marcha atras
unsigned int id_motor = 0x01;                                     //ID asignado a el dispositivo de servo motor en la red can

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string


void loop() {
  char dato = Serial.read();                                      //Captura la informacion que llega desde el bluetooth en la variable dato

  switch(dato){
    case 'A':{
      Serial.println("Recibi una A");                             //Confirma en el serial la llegada de una A
      angulo+=90;
      angulo= constrain(angulo,0,180);
      miServo.write(angulo);Serial.println("Envie servo a la derecha");
      break;
    }
    case 'B':{
      Serial.println("Recibi una B");                             //Confirma en el serial la llegada de una B
      angulo-=90;
      angulo= constrain(angulo,0,180);
      miServo.write(angulo);Serial.println("Envie servo a la izquierda");
      break;
    }
    case 'C':{
      Serial.println("Recibi una C");                             //Confirma en el serial la llegada de una C
      Serial.println("Envie motor adelante");
      digitalWrite(posmotor_adelante, HIGH);
      digitalWrite(posmotor_atras, LOW);
      analogWrite(potenciamotor_ENB,200);                         //Potencia del motor a 200
      break;
    }
    case 'D':{
      Serial.println("Recibi una D");                             //Confirma en el serial la llegada de una D
      Serial.println("Envie motor atras");
      digitalWrite(posmotor_adelante, LOW);
      digitalWrite(posmotor_atras, HIGH);
      analogWrite(potenciamotor_ENB,200);                         //Potencia del motor a 200
      break;
    }
    case 'E':{
      Serial.println("Recibi una E");                             //Confirma en el serial la llegada de una E
      Serial.println("Envie motor parado");
      digitalWrite(posmotor_adelante, LOW);
      digitalWrite(posmotor_atras, LOW);
      analogWrite(potenciamotor_ENB,0);
      break;
    }
  }

  if(!digitalRead(CAN0_INT)){
    Serial.println("leyendo mensaje");
    CAN.readMsgBuf(&rxId,&len,rxBuf);           //leer len=tmaño de datos,buf= bytes de datos
    Serial.println(rxId);
    if(true){
      if(rxBuf[7]== 2){
        digitalWrite(posmotor_adelante, LOW);
        digitalWrite(posmotor_atras, LOW);
        analogWrite(potenciamotor_ENB,0);
      }
      else{
        digitalWrite(posmotor_adelante, LOW);
        digitalWrite(posmotor_atras, LOW);
        analogWrite(potenciamotor_ENB,0);
      }
    }
  }
}
