#ifndef ControladorLora_h
#define ControladorLora_h
#define BAND 915E6

#if (ARDUINO>=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class ControladorLora{
  
  public:
    ControladorLora(); /*Constructor*/
    void verificarConexion();
    void iniciarTransmision();
    void finalizarTransmision();
    void enviarDato( String y,double x);
};




#endif
