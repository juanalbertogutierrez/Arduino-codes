#include <mcp_can.h>                          //Biblioteca para la integración del BUS CAN Shield con arduino
#include <SPI.h>                              //Permite al arduino comunicarse con dispositivos SPI (Serial Peripheral Interface)

#include <Servo.h>

Servo miServo;
int angulo= 90;

int posmotor_derecha=5;                       //Pin para direccion del motor derecha 
int posmotor_izquierda=4;                     //Pin para direccion del motor izquierda
int potenciamotor_ENB=3;                      //Pin para potencia del motor
                                              // the cs pin of the version after v1.1 is default to D9
                                              // v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;                     //Pin que usara el can shield para el envió/recepción de mensajes de la red can
MCP_CAN CAN(SPI_CS_PIN);                      // Configura el pin 9 (SPI_CS_PIN) para la red bus can


void setup(){
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
  miServo.attach(7);                          //Indicamos que el servo motor trabajara en el pin 7
  digitalWrite(posmotor_derecha, OUTPUT);     //Indicamos que el pin 5 es de salida
  digitalWrite(posmotor_izquierda, OUTPUT);     //Indicamos que el pin 4 es de salida
  digitalWrite(potenciamotor_ENB, OUTPUT);     //Indicamos que el pin 3 es de salida
}

unsigned char servo_derecha[8] =   {0, 0, 0, 0, 0, 0, 0, 0};     //Mensaje personalizado para indicar que el servo motor va a girar a la derecha
unsigned char servo_izquierda[8] = {0, 0, 0, 0, 0, 0, 0, 1};     //Mensaje personalizado para indicar que el servo motor va a girar a la izquierda

unsigned char motor_adelante[8] =  {0, 0, 0, 0, 0, 0, 0, 0};     //Mensaje personalizado para indicar que el motor dara marcha adelante
unsigned char motor_atras[8] =     {0, 0, 0, 0, 0, 0, 0, 1};     //Mensaje personalizado para indicar que el motor dara marcha atras
unsigned char motor_parado[8] =    {0, 0, 0, 0, 0, 0, 0, 2};     //Mensaje personalizado para indicar que el motor dara marcha atras

unsigned int id_servo = 0x00;                                     //ID asignado a el dispositivo de servo motor en la red can
unsigned int id_motor = 0x01;                                     //ID asignado a el dispositivo de servo motor en la red can


void loop(){
	
	char dato = Serial.read();                                      //Captura la informacion que llega desde el bluetooth en la variable dato

  switch(dato){
    case 'A':{
      Serial.println("Recibi una A");                             //Confirma en el serial la llegada de una A
      CAN.sendMsgBuf(id_servo, 0, 8, servo_derecha);              //Envia el mensaje servo derecha con el respectivo id
      Serial.println("Envie servo a la derecha");
      angulo+=90;
      angulo= constrain(angulo,0,180);
      miServo.write(angulo);
      break;
    }
    case 'B':{
      Serial.println("Recibi una B");                             //Confirma en el serial la llegada de una B
      CAN.sendMsgBuf(id_servo, 0, 8, servo_izquierda);            //Envia el mensaje servo izquierda con el respectivo id
      Serial.println("Envie servo a la izquierda");
      angulo-=90;
      angulo= constrain(angulo,0,180);
      miServo.write(angulo);
      break;
    }
    case 'C':{
      Serial.println("Recibi una C");                             //Confirma en el serial la llegada de una C
      CAN.sendMsgBuf(id_motor, 0, 8, motor_adelante);             //Envia el mensaje motor adelante con el respectivo id
      Serial.println("Envie motor a la adelante");
      digitalWrite(posmotor_derecha, HIGH);
      digitalWrite(posmotor_izquierda, LOW);
      analogWrite(potenciamotor_ENB,180);                         //Potencia del motor a 200
      break;
    }
    case 'D':{
      Serial.println("Recibi una D");                             //Confirma en el serial la llegada de una D
      CAN.sendMsgBuf(id_motor, 0, 8, motor_atras);                //Envia el mensaje motor atras con el respectivo id
      Serial.println("Envie motor a la atras");
      digitalWrite(posmotor_derecha, LOW);
      digitalWrite(posmotor_izquierda, HIGH);
      analogWrite(potenciamotor_ENB,180);                         //Potencia del motor a 200
      break;
    }
    case 'E':{
      Serial.println("Recibi una E");                             //Confirma en el serial la llegada de una E
      CAN.sendMsgBuf(id_motor, 0, 8, motor_parado);                //Envia el mensaje motor parado con el respectivo id
      Serial.println("Envie motor a la parado");
      digitalWrite(posmotor_derecha, LOW);
      digitalWrite(posmotor_izquierda, LOW);
      analogWrite(potenciamotor_ENB,0);
      break;
    }
  }
}
