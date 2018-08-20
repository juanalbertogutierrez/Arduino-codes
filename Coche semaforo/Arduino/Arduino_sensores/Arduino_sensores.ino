/*  Aplicacion arduino
 *  Proyecto arduino- Estacionado Automatico
 *  Servo motor y sensores delanteros.
 *  Juan Alberto Gutierrez Canto Ingeniero en Software
 *  enero 6 del 2017. version 1.0
*/

#include <mcp_can.h>                          //Biblioteca para la integración del BUS CAN Shield con arduino
#include <SPI.h>                              //Permite al arduino comunicarse con dispositivos SPI (Serial Peripheral Interface)
#define CAN0_INT 2                              // Set INT to pin 2
#define dfPecho 3
#define dfPtrig 4
#define dlPecho 5
#define dlPtrig 6
#define tfPecho 7
#define tfPtrig 8
#define tlPecho 10
#define tlPtrig 11

long dfDuracion, dfDistancia,dlDuracion, dlDistancia,tfDuracion, tfDistancia,tlDuracion, tlDistancia;
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
  pinMode(dfPecho, INPUT);
  pinMode(dfPtrig, OUTPUT);
  pinMode(dlPecho, INPUT);
  pinMode(dlPtrig, OUTPUT);
  pinMode(tfPecho, INPUT);
  pinMode(tfPtrig, OUTPUT);
  pinMode(tlPecho, INPUT);
  pinMode(tlPtrig, OUTPUT);
  pinMode(13,1);
  //CAN.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

unsigned char motor_parado[8] =    {0, 0, 0, 0, 0, 0, 0, 2};     //Mensaje personalizado para indicar que el motor dara marcha atras
unsigned int id_motor = 0x01;                                     //ID asignado a el dispositivo de servo motor en la red can


void loop() {
  CAN.sendMsgBuf(id_motor, 0, 8, motor_parado);              //Envia el mensaje motor parado con el respectivo id
    Serial.println("mando mensaje");
  digitalWrite(dfPtrig,LOW);
  digitalWrite(dlPtrig,LOW);
  digitalWrite(tfPtrig,LOW);
  digitalWrite(tlPtrig,LOW);
  //delay(2000);
  digitalWrite(dfPtrig,HIGH);
  digitalWrite(dlPtrig,HIGH);
  digitalWrite(tfPtrig,HIGH);
  digitalWrite(tlPtrig,HIGH);
  //delay(2000);
  digitalWrite(dfPtrig,LOW);
  digitalWrite(dlPtrig,LOW);
  digitalWrite(tfPtrig,LOW);
  digitalWrite(tlPtrig,LOW);

  dfDuracion = pulseIn(dfPecho,HIGH);
  dfDistancia=(dfDuracion/2)/29;
  dlDuracion = pulseIn(dlPecho,HIGH);
  dlDistancia=(dlDuracion/2)/29;
  tfDuracion = pulseIn(tfPecho,HIGH);
  tfDistancia=(tfDuracion/2)/29;
  tlDuracion = pulseIn(tlPecho,HIGH);
  tlDistancia=(tlDuracion/2)/29;
  if(dfDistancia >= 500 || dfDistancia <= 0 || dlDistancia >= 500 || dlDistancia <= 0 || tfDistancia >= 500 || tfDistancia <= 0 || tlDistancia >= 500 || tlDistancia <= 0){
    Serial.println("---");
  }
  else{
    Serial.print(dfDistancia);
    Serial.println("cm");
    digitalWrite(13,0);
  }

  if((dfDistancia <= 10 && dfDistancia >= 1) || (dlDistancia <= 10 && dlDistancia >= 1) || (tfDistancia <= 10 && tfDistancia >= 1) || (tlDistancia <= 10 && tlDistancia >= 1)){
    digitalWrite(13,1);
    Serial.println("Alarma.....");
    CAN.sendMsgBuf(id_motor, 0, 8, motor_parado);              //Envia el mensaje motor parado con el respectivo id
    Serial.println("mando mensaje");
    Serial.println(id_motor);
  }
  delay(2000);

}
