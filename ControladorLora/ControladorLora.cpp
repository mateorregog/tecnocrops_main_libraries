#include "ControladorLora.h"
#include <LoRa.h>

ControladorLora::ControladorLora(){  //Constructor
}

void ControladorLora::verificarConexion(){
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
}

void ControladorLora::iniciarTransmision(){
  LoRa.beginPacket();
}

void ControladorLora::finalizarTransmision(){
  LoRa.endPacket();
}

void ControladorLora::enviarDato( String y,double x){
  LoRa.print(y); //Envia encabezado identificador de pH
  LoRa.print("|");
  LoRa.print(x);
  LoRa.print("|");
}
