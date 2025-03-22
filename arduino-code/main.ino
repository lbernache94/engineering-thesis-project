#include <Servo.h>                   // Librería de servo
#include <Wire.h>                    // Librería para utilizar I2C
#include <Adafruit_ADS1015.h>       // Librería para utilizar ADC de 16 bits

Servo ESC;                           // Función para habilitar el motor

Adafruit_ADS1115 ads;                // Función que indica que los datos del ADC serán utilizados
const float multiplier = 0.1875F;    // Como constantes flotantes

int vel = 1000;                      // Este es un dato que indica las aceleraciones del motor
char puerto = 0;

volatile byte RPMcontador;
unsigned int RPM;
unsigned long tiempoAtras;

void funcionRPM() {
  RPMcontador++;
}

void setup() {
  // Esta parte del código indica que entre el dato "1000" y el dato "2000"
  // estarán las variaciones de velocidad del motor, siendo "1000" el que lo active
  ESC.attach(9);                     // en el puerto. Mismo que tiene un retardo de dos segundos.
  ESC.writeMicroseconds(2000);      // 1000 = 1ms
  delay(2000);

  ESC.writeMicroseconds(1000);
  delay(5000);                       // Una vez iniciado el programa y sincronizado con labVIEW 5 segundos después
                                    // comenzará a funcionar el motor.

  Serial.begin(9600);               // Se inicializa el puerto serial para la comunicación con la PC,
  Serial.setTimeout(10);            // Inician las interrupciones mismas que son utilizadas para medir
  ads.begin();                      // las revoluciones a las que gira el motor.
  attachInterrupt(0, funcionRPM, FALLING);
  RPMcontador = 0;
  RPM = 0;
  tiempoAtras = 0;
}

void loop() {
  // En esta parte del código se indica que el ADC será utilizado en modo diferencial
  attachInterrupt(0, funcionRPM, FALLING);

  int16_t results = ads.readADC_Differential_0_1();

  if (Serial.available() > 0) {
    puerto = Serial.read();          // El motivo por el cual se utilizará casos en esta parte del código
                                     // es porque para la realización de la comunicación microcontrolador-labVIEW
                                     // resulta más sencillo enviar un dato, en este caso un caracter por el puerto serial,
                                     // para así indicar al software qué instrucción ejecutar y procesar.
    switch (puerto) {
      case 'c':
        Serial.print(results * (-1));
        attachInterrupt(0, funcionRPM, FALLING);
        break;

      case 'b':
        // tiempoAtras = millis();
        // RPM = 15 * 1000 / (millis() - tiempoAtras) * RPMcontador;
        Serial.print(RPMcontador);
        attachInterrupt(0, funcionRPM, FALLING);
        // RPMcontador = 0;
        break;

      case 'a':
        while (1) {
          if (Serial.available() > 0) break;
          attachInterrupt(0, funcionRPM, FALLING);
        }
        vel = Serial.parseInt();     // Leer un entero por serial
        if (vel != 0) {
          ESC.writeMicroseconds(vel); // Generar un pulso con el número recibido
        }
        break;
    }
  }

  detachInterrupt(0);
}
