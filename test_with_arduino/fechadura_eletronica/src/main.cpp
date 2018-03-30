/*  Programa para testar a fechadura eletrônica com Arduino
 *  Escrito por Will Douglas
 *
 *  Hardware:
 *  01 Fechadura eletrônica
 *  01 Arduino UNO
 *  01 Módulo Relé
 *  Jumpers
 */

//<--- Bibliotecas --->
#include <Arduino.h>

//<--- Pinos do MCU --->
const unsigned int fechadura_001 = 6;

//<--- Função principal --->
void setup() {
    Serial.begin(9600);
    pinMode(fechadura_001, OUTPUT);
}//end setup

//<--- Loop infinito --->
void loop() {
    while(Serial.available()) {
        char a = (char)Serial.read();
        switch(a) {
            case 'a':
                Serial.println("Aberto");
                digitalWrite(fechadura_001, LOW);
                delay(50);
                digitalWrite(fechadura_001,HIGH);
                break;
            case 's':
                Serial.println("Fechado");
                digitalWrite(fechadura_001, HIGH);
                break;
        }//end switch
    }//end while
}//end loop
