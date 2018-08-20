/*  Aplicacion arduino
 *  Proyecto arduino- Estacionado Automatico
 *  Servo motor y sensores delanteros.
 *  Juan Alberto Gutierrez Canto Ingeniero en Software
 *  enero 5 del 2017. version 3.1
*/

#include <mcp_can.h>                          //Biblioteca para la integración del BUS CAN Shield con arduino
#include <SPI.h>                              //Permite al arduino comunicarse con dispositivos SPI (Serial Peripheral Interface)
#include <Servo.h>
#define CAN0_INT 2                              // Set INT to pin 2

Servo miServo;
int servo_PIN= 7;
int angulo= 90;

                                              // the cs pin of the version after v1.1 is default to D9
                                              // v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;                     //Pin que usara el can shield para el envió/recepción de mensajes de la red can
MCP_CAN CAN(SPI_CS_PIN);                      // Configura el pin 9 (SPI_CS_PIN) para la red bus can



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
  miServo.attach(servo_PIN);                          //Indicamos que el servo motor trabajara en el pin 7
  CAN.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  pinMode(CAN0_INT,INPUT);                    //entrada de mensaje can
}

unsigned char servo_derecha[8] =   {0, 0, 0, 0, 0, 0, 0, 0};     //Mensaje personalizado para indicar que el servo motor va a girar a la derecha
unsigned char servo_izquierda[8] = {0, 0, 0, 0, 0, 0, 0, 1};     //Mensaje personalizado para indicar que el servo motor va a girar a la izquierda

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string


void loop() {
  if(!digitalRead(CAN0_INT)){
    CAN.readMsgBuf(&rxId,&len,rxBuf);           //leer len=tmaño de datos,buf= bytes de datos
    Serial.println(rxId);
    if(rxId == 0x00){
      if(rxBuf[7]==0){
        angulo+=90;
        angulo= constrain(angulo,0,180);
        miServo.write(angulo);
      }
      else{
        angulo-=90;
        angulo= constrain(angulo,0,180);
        miServo.write(angulo);
      }
    }
  }

}
