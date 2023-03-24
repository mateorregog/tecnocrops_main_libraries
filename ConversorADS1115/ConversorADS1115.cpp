//Asignación de los bits de configuración

#include <Wire.h>
#include <OneWire.h> //Librerías  sensor temperatura agua
#include <DallasTemperature.h> //Sensor temperatura agua
#include "ConversorADS1115.h"

ConversorADS1115::ConversorADS1115() {

  _a = 0b00000000000000000;
  _b = 0b00000000;
  _c = 0b00000000;
  _lec = 0b00000000000000000;
  _res = 0;
  Wire.begin();
}

void ConversorADS1115::reestablecerValores() {
  _b = 0b00000000;
  _c = 0b00000000;
  _res = 0;
}

//OS: Operational status/single-shot conversion start. Números 0 o 1.
void ConversorADS1115::bit15(uint8_t x) {
  uint8_t temp = x << 7; //Corrimiento de bits
  _b = _b | temp;
}

//Input multiplexer configuration. En decimal números de 0 a 7. (Consultar datasheet para ver la configuración detallada) 4 = A0; 5 = A1 ; 6 = A2 ; 7 = A3
void ConversorADS1115::bit14a12(uint8_t x) {
  uint8_t temp = x << 4;
  _b = _b | temp;
}

// Programmable gain amplifier configuration. Números de 0 a 7 donde:
// 0 : FS = ±6.144V(1), 1 : FS = ±4.096V(1), 2 : FS = ±2.048V (default), 3 : FS = ±1.024V,  4 : FS = ±0.512V, 5 : FS = ±0.256V, 6 : FS = ±0.256V, 7 : FS = ±0.256V
void ConversorADS1115::bit11a9(uint8_t x) {
  uint8_t temp = x << 1;
  _b = _b | temp;
}

// Device operating mode. 0 : Continuo , 1:Power-down single-shot (default)
void ConversorADS1115::bit8(uint8_t x) {
  uint8_t temp = x;
  _b = _b | temp;
}

// Data rate. Números de 0 a 7 donde:
//0 : 8SPS , 1 : 16SPS , 2 : 32SPS , 3 : 64SPS, 4 : 128SPS (default), 5 : 250SPS, 6 : 475SPS, 7 : 860SPS
void ConversorADS1115::bit7a5(uint8_t x) {
  uint8_t temp = x << 5;
  _c = _c | temp;
}

//Comparator mode. Números 0 o 1.
// 0 : Comparador tradicional con hysteresis (default), 1 : Comparador de ventana (Window comparator)
void ConversorADS1115::bit4(uint8_t x) {
  uint8_t temp = x << 4;
  _c = _c | temp;
}

//Comparator polarity. Números 0 o 1.
// 0 : Active low , 1 : Active high
void ConversorADS1115::bit3(uint8_t x) {
  uint8_t temp = x << 3;
  _c = _c | temp;
}

//Latching comparator. Números 0 o 1.
//0 : Non latching comparator (Default), 1 : Lartching comparator
void ConversorADS1115::bit2(uint8_t x) {
  uint8_t temp = x << 2;
  _c = _c | temp;
}

//Comparator queue and disable check
void ConversorADS1115::bit1a0(uint8_t x) {
  uint8_t temp = x;
  _c = _c | temp;
}

//Crea la cadena de 2 bytes check
void ConversorADS1115::formar2bytes() {
  _a = _b << 8;
  _a = _a | _c;
}

void ConversorADS1115::configuracionInicialAds(uint8_t x) { //check
  bit15(0);
  bit14a12(x);
  bit11a9(0);
  bit8(0);
  bit7a5(1);
  bit4(0);
  bit3(0);
  bit2(0);
  bit1a0(0);
  formar2bytes();
}

void ConversorADS1115::configurarADS1115() {
  Wire.beginTransmission(ADS2); //Direccion I2C que sera invariable que es VDD
  Wire.write(REGC);
  Wire.write(_b);
  Wire.write(_c);
  Wire.endTransmission();
}

uint16_t ConversorADS1115::conocerConfiguracion() {   //Función para conocer la configuración del ADS
  Wire.beginTransmission(ADS2);
  Wire.write(REGC);
  Wire.endTransmission();
  Wire.requestFrom(ADS2, 2);
  _lec = Wire.read() << 8 | Wire.read();
  /*Serial.print("Lectura registro configuración : ");
    Serial.println(lec,BIN);*/
  return _lec;
}

double ConversorADS1115::obtenerDatos() {
  Wire.beginTransmission(ADS2);
  Wire.write(REGL);
  Wire.endTransmission();
  Wire.requestFrom(ADS2, 2);
  _lec = Wire.read() << 8 | Wire.read();
  /*Serial.print("La lectura del sensor es: ");*/
  _res = (_lec * 6.144) / (32768);
  /*Serial.print(res,6);
    Serial.println(" Voltios");*/
  return _res;
}
