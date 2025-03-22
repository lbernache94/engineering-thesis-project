.#include<Servo.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Servo ESC;


Adafruit_ADS1115 ads;
const float multiplier = 0.1875F;

int vel = 1000;
char puerto = 0;

volatile byte RPMcontador;
unsigned int RPM;
unsigned long tiempoAtras;

void funcionRPM()
{
  RPMcontador++;
}

void setup()
{
  ESC.attach(9);
  ESC.writeMicroseconds(2000); //1000 = 1ms
  delay(2000);
  ESC.writeMicroseconds(1000);

  delay(5000);

  Serial.begin(9600);
  Serial.setTimeout(10);
  ads.begin();
  attachInterrupt(0, funcionRPM, FALLING);
  RPMcontador = 0;
  RPM = 0;
  tiempoAtras = 0;
}

void loop()
{


  //Serial.print("RP´s:  ");
  //Serial.println(RPM/60);
  attachInterrupt(0, funcionRPM, FALLING);

  int16_t results = ads.readADC_Differential_0_1();

  //Serial.println("DATOS DE ADC ");
  //Serial.print(results * (-1));
  //Serial.print(" ");
  //Serial.print(results * multiplier* (-1));
  //Serial.println("mV");

  //delay(1000);
  if (Serial.available() >0)
  {
    puerto = Serial.read();
    attachInterrupt(0, funcionRPM, FALLING);
    switch (puerto)
    {
      case 'c':
        Serial.print(results * (-1));
        attachInterrupt(0, funcionRPM, FALLING);
        break;

      case 'b':
        //tiempoAtras = millis();
        //RPM = 15 * 1000 / (millis() - tiempoAtras) * RPMcontador;
        Serial.print(RPMcontador);
        attachInterrupt(0, funcionRPM, FALLING);
        //RPMcontador = 0;
        break;

      case 'a':
        while (1)
        {
          if (Serial.available() > 0) break;
          attachInterrupt(0, funcionRPM, FALLING);
        }
        vel = Serial.parseInt(); //Leer un entero por serial
        if (vel != 0)
        {
          ESC.writeMicroseconds(vel); //Generar un pulso con el numero recibido
        }
    }
    //vel = Serial.parseInt(); //Leer un entero por serial
    //if(vel != 0)
    //{
    // ESC.writeMicroseconds(vel); //Generar un pulso con el numero recibido
    //}
  }
  detachInterrupt (0);
}
