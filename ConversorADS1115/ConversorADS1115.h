#ifndef ConversorADS1115_h
#define ConversorADS1115_h

#include <Wire.h>

#if (ARDUINO>=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class ConversorADS1115{
  public:
   ConversorADS1115(); /* //Constructor*/
   void configuracionInicialAds(uint8_t x);/*//Metodo publico de configuracion bit a bit del conversor*/
   void reestablecerValores();
   void configurarADS1115();
   uint16_t conocerConfiguracion();
   double obtenerDatos();
    
  private:
   void bit15(uint8_t x);
   void bit14a12(uint8_t y);
   void bit11a9(uint8_t x);
   void bit8(uint8_t x);
   void bit7a5(uint8_t x);
   void bit4(uint8_t x);
   void bit3(uint8_t x);
   void bit2(uint8_t x);    
   void bit1a0(uint8_t x);
   void formar2bytes();


    /*//Dirección I2C del ADS115, puede ser 0X48, 0X49, 0X4A, 0X4B*/
   const uint8_t ADS1 = 0x48;  /*//Ground*/
   const uint8_t ADS2 = 0x49;/*//VDD*/ 
   const uint8_t ADS3 = 0x4A; /*//SDA*/
   const uint8_t ADS4 = 0x4B; /*//SCL*/

    //Registros de acceso
   const uint8_t REGL = 0x00; /* //Registro de conversión*/
   const uint8_t REGC = 0x01;  /*//Registro de configuración del ADS1115*/
   const uint8_t REGLT = 0x02; /*//Registro bajo de disparo de alerta*/
   const uint8_t REGHT = 0x03; /*//Registro alto de disparo de alerta*/
  
    /*//Variables de los bits a configurar*/
   uint16_t _a;
   uint8_t _b;
   uint8_t _c;
   uint16_t _lec;
   double _res;
  
};

#endif


  
