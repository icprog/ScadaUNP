/*
*Automação e controle do Laboratório BatCaverna
*Autor: Will Douglas
*/
//<--- Bibliotecas --->
#include <Arduino.h>
#include <cstdlib>
#include <time.h>
#include <Modbus.h>
#include <ModbusIP_ESP32.h>


//<--- Configuração da Pinagem do ESP-32 --->
const unsigned int dht_001 = 17;

//<--- Modbus register offsets --->
const unsigned int mb_reg_dht_temperatura_001 = 3;
const unsigned int mb_reg_dht_temperatura_002 = 4;
const unsigned int mb_reg_dht_umidade_001 = 5;
const unsigned int mb_reg_dht_umidade_002 = 6;

//<--- Variáveis globais --->
unsigned int tempo=0;
//<--- Objetos --->
ModbusIP mb;

//<--- Cabeçalhos de funções --->
void float_to_word_array(float, word *);
float numero_randomico();


void setup()
{
	srand(time(NULL));
	mb.config("ScadaBR", "ScadaBRunp");	//"ssid", "senha"
	//dht.begin();

	//Adicionando registradores do Modbus
	mb.addIreg(mb_reg_dht_temperatura_001);
	mb.addIreg(mb_reg_dht_umidade_001);
	mb.addIreg(mb_reg_dht_temperatura_002);
	mb.addIreg(mb_reg_dht_umidade_002);
} //end setup


void loop()
{
	mb.task();
	//necessário para converter valores float para a leitura no modbus.
	if((millis()-tempo) < 2000){
		tempo = millis();
		word reg[2];
		float_to_word_array(numero_randomico(), reg);
		mb.Ireg(mb_reg_dht_temperatura_001, reg[1]);
		mb.Ireg(mb_reg_dht_temperatura_002, reg[0]);
		float_to_word_array(numero_randomico(), reg);
		mb.Ireg(mb_reg_dht_umidade_001, reg[1]);
		mb.Ireg(mb_reg_dht_umidade_002, reg[0]);
	}
} //end loop


void float_to_word_array(float numero, word *reg)
{
	byte* array;
	array = (byte*) &numero;
	reg[0] = (array[1]<<8) | array[0];
	reg[1] = (array[3]<<8) | array[2];
} //end float_to_word_array


float numero_randomico()
{
	float numero;
	numero = rand() % 100;
	numero += (rand() % 100)*0.1;
	return numero;
}
