#include <ControladorLora.h> //Libreria controlar LoRa
#include <ConversorADS1115.h> //Librería conversor ADS1115
#include <OneWire.h> //Librerías  sensor temperatura agua
#include <DallasTemperature.h> //Sensor temperatura agua

/*Creación de tareas*/
TaskHandle_t Tarea1; //Manejador de la tarea u objeto PH
TaskHandle_t Tarea2; //Manejador de la tarea u objeto TDS
TaskHandle_t Tarea3; //Manejador de la tarea u objeto Temperatura Agua
TaskHandle_t Tarea4; //Manejador de la tarea u objeto Luz
TaskHandle_t Tarea5; //Manejador de la tarea u objeto LoRa

int contador; //Cuenta las emisiones lora
double temperatura, pH, tds, luz; //Almacena los valores de las variables a medir
uint16_t confReg;  //Almacena la configuración de registro
const int oneWireBus = 13; // GPIO where the DS18B20 is connected to

OneWire oneWire(oneWireBus);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature sensor 
ConversorADS1115 conversor1; //Instancia el objeto conversor 
ControladorLora controlador1; //Instancia el objeto controlador de LoRa

void setup() {
  Serial.begin(9600);
  sensors.begin(); // Start the DS18B20 sensor
  conversor1.configuracionInicialAds(4); //Escribe el registro para leer pH en el A0
  conversor1.configurarADS1115(); // Configura el ads con el nuevo registro
  controlador1.verificarConexion(); //Verifica conexión LoRa
  

  xTaskCreatePinnedToCore(
    lectura_pH,//Función que implementa la tarea
    "Tarea1", //Nombre de la tarea
    10000, //Tamaño de la pila en palabras
    NULL, //Parámetros de entrada de la tarea
    10, //Prioridad de la tarea (0 es la más baja)
    &Tarea1, //Objeto creado para la tarea
    0); //Núcleo en el cual correrá la tarea

   xTaskCreatePinnedToCore(
    lectura_TDS,//Función que implementa la tarea
    "Tarea2", //Nombre de la tarea
    10000, //Tamaño de la pila en palabras
    NULL, //Parámetros de entrada de la tarea
    8, //Prioridad de la tarea (0 es la más baja)
    &Tarea2, //Objeto creado para la tarea
    0); //Núcleo en el cual correrá la tarea

   xTaskCreatePinnedToCore(
    lectura_Temperatura,//Función que implementa la tarea
    "Tarea3", //Nombre de la tarea
    10000, //Tamaño de la pila en palabras
    NULL, //Parámetros de entrada de la tarea
    6, //Prioridad de la tarea (0 es la más baja)
    &Tarea3, //Objeto creado para la tarea
    0); //Núcleo en el cual correrá la tarea

   xTaskCreatePinnedToCore(
    lectura_luz,//Función que implementa la tarea
    "Tarea4", //Nombre de la tarea
    10000, //Tamaño de la pila en palabras
    NULL, //Parámetros de entrada de la tarea
    4, //Prioridad de la tarea (0 es la más baja)
    &Tarea4, //Objeto creado para la tarea
    0); //Núcleo en el cual correrá la tarea

    xTaskCreatePinnedToCore(
    envio_lora,//Función que implementa la tarea
    "Tarea5", //Nombre de la tarea
    10000, //Tamaño de la pila en palabras
    NULL, //Parámetros de entrada de la tarea
    1, //Prioridad de la tarea (0 es la más baja)
    &Tarea5, //Objeto creado para la tarea
    1); //Núcleo en el cual correrá la tarea
  
    Serial.begin(9600);   
}

void loop() {
    Serial.println("Núcleo: "+ String(xPortGetCoreID()));
    delay(4000);   
}

void lectura_pH(void * parameter){
  for(;;){
    Serial.println("Sensor PH: ");
    pH = conversor1.obtenerDatos(); //Duelve el valor de la lectura. 
    confReg = conversor1.conocerConfiguracion();//Devuelve  el registro de configuración para ver qué entrada está leyendo.
    conversor1.configuracionInicialAds(5); //Escribe el registro para leer tds en el A1
    conversor1.configurarADS1115(); // Configura el ads con el nuevo registro
    Serial.print("La lectura del sensor es: ");
    Serial.print(pH,6);
    Serial.println(" Voltios");
    conversor1.reestablecerValores(); //Limpia los valores de la lectura pH
    delay(5000);
  }
}

void lectura_TDS(void * parameter){
  for(;;){   
    Serial.println("Sensor TDS: ");
    tds = conversor1.obtenerDatos(); //Obtiene el valor de la lectura.
    /*conversion_tds();*/
    confReg = conversor1.conocerConfiguracion();//Devuelve registro de configuración para ver qué entrada está leyendo.
    conversor1.configuracionInicialAds(6); //Escribe el registro para leer luz en el A2
    conversor1.configurarADS1115(); // Configura el ads con el nuevo registro
    Serial.print("La lectura del sensor es: ");
    Serial.print(tds,6);
    Serial.println(" Voltios");
    conversor1.reestablecerValores();
    delay(5000);
  }
}

void lectura_luz(void * parameter){
  for(;;){   
    Serial.println("Sensor Luz: ");
    luz = conversor1.obtenerDatos(); //Obtiene el valor de la lectura.
    confReg = conversor1.conocerConfiguracion();//Devuelve registro de configuración para ver qué entrada está leyendo.
    conversor1.configuracionInicialAds(4); //Escribe el registro para leer pH en A0
    conversor1.configurarADS1115(); // Configura el ads con el nuevo registro
    Serial.print("La lectura del sensor es: ");
    Serial.print(luz,6);
    Serial.println(" Voltios");
    conversor1.reestablecerValores();
    delay(5000);
  }
}

void lectura_Temperatura(void * parameter){
  for(;;){   
    Serial.println("Sensor Temperatura: ");
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    delay(5000);
  }
}

/*
double conversion_tds(){
  double x=(res*1000)/3.3;
  Serial.println("El valor de PPM es: "+String(x));
}*/

void envio_lora(void * parameter){
  for(;;){   
    Serial.print("Enviando paquete: ");
    Serial.println(contador);
    controlador1.iniciarTransmision(); //Inicia transmisión LoRa
    controlador1.enviarDato("p",pH); //Envia dato de pH con encabezado "p"
    delay(2000);
    controlador1.enviarDato("t",tds);
    delay(2000);
    controlador1.enviarDato("m",temperatura);
    delay(2000);
    controlador1.enviarDato("l",luz);
    delay(2000);
    controlador1.finalizarTransmision(); //Finaliza tranmisión LoRa
    contador++;
    luz=0;tds=0;pH=0;temperatura=0;
  }
}
