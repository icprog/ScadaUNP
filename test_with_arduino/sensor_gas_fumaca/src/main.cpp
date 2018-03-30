/*  Programa para testar o sensor de gás com Arduino
 *  Escrito por Will Douglas
 *
 *  Hardware:
 *  01 sensor de gás MQ-2 <Datasheet = http://img.filipeflop.com/files/download/Datasheet_Sensor_Gas_MQ2.pdf>
 *  01 Arduino UNO
 *  Jumpers
 */

//<--- Bibliotecas --->
#include <Arduino.h>

//<--- Pinos do MCU --->
const unsigned int sensor_gas_001 = A2;

//<--- Função principal --->
void setup() {
    Serial.begin(9600);
    pinMode(sensor_gas_001, INPUT);
}//end setup

//<--- Loop infinito --->
void loop() {
    Serial.println(analogRead(sensor_gas_001));
}//end loop
