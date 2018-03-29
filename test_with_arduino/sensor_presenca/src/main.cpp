/*  Programa para testar o sensor de presença com arduino Arduino
 *  Escrito por Will Douglas
 *
 *  Hardware:
 *  01 sensor de presença DYP-ME003 <Datasheet = http://elecfreaks.com/store/download/datasheet/sensor/DYP-ME003/Specification.pdf>
 *  01 Arduino UNO
 *  Jumpers
 */

//<--- Bibliotecas --->
#include <Arduino.h>

//<--- Pinos do MCU --->
const unsigned int sensor_presenca_001 = 4;

//<--- Variáveis globais --->
unsigned int tempo = 0;
bool status;

//<--- Função principal --->
void setup() {
    Serial.begin(9600);
    pinMode(sensor_presenca_001, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

} //end setup

//<--- Loop infinito --->
void loop() {
    if((millis() - tempo) > 2000){ //equivalente a delay(2000);
      tempo = millis();
      status = digitalRead(sensor_presenca_001);
      Serial.println(status);
      digitalWrite(LED_BUILTIN, status);
    }
} //end loop
