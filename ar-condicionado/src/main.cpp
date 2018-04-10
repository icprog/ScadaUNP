/*
*Automação e controle do Laboratório BatCaverna
*Autor: Will Douglas
*/

//<--- Bibliotecas --->
#include <Arduino.h>
#include <Modbus.h>
#include <ModbusIP_ESP32.h>
#include <DHT.h>


//<--- Configuração da Pinagem do ESP-32 --->
const unsigned int ar_condicionado_001 = 33;
const unsigned int lampada_001 = 17;
const unsigned int dht_001 = 25;
const unsigned int gas_001 = 18;
const unsigned int presenca_001 = 19;

//<--- Modbus register offsets --->
const unsigned int mb_reg_ar_condicionado_001 = 1;
const unsigned int mb_reg_lampada_001 = 2;
const unsigned int mb_reg_dht_temperatura_001 = 3;
const unsigned int mb_reg_dht_temperatura_002 = 4;
const unsigned int mb_reg_dht_umidade_001 = 5;
const unsigned int mb_reg_dht_umidade_002 = 6;
const unsigned int mb_reg_gas_001 = 7;
const unsigned int mb_reg_presenca_001 = 8;

//<--- Objetos --->
ModbusIP mb;
DHT dht(dht_001, DHT22);

//<--- Cabeçalhos de funções --->
void float_to_word_array(float, word *);

//<--- Variáveis globais --->
unsigned int tempo = 0;


void setup()
{
	mb.config("ScadaBR", "ScadaBRunp");	//"ssid", "senha"
	dht.begin();

	//setup dos pinos
	pinMode(ar_condicionado_001, OUTPUT);
	pinMode(lampada_001, OUTPUT);

	//Adicionando registradores do Modbus
	mb.addCoil(mb_reg_ar_condicionado_001);
	mb.addCoil(mb_reg_lampada_001);
	mb.addIreg(mb_reg_dht_temperatura_001);
	mb.addIreg(mb_reg_dht_umidade_001);
	mb.addIreg(mb_reg_dht_temperatura_002);
	mb.addIreg(mb_reg_dht_umidade_002);
	mb.addIreg(mb_reg_gas_001);
	mb.addIsts(mb_reg_presenca_001);
} //end setup


void loop()
{
	mb.task();

	digitalWrite(ar_condicionado_001, !mb.Coil(mb_reg_ar_condicionado_001)); //envia 0v para acionar relé
	digitalWrite(lampada_001, !mb.Coil(mb_reg_lampada_001)); //envia 0v para acionar relé


	//necessário para converter valores float para a leitura no modbus.
	if((millis()-tempo) < 2000){
		tempo = millis();
		word reg[2];
		float_to_word_array(dht.readTemperature(), reg);
		mb.Ireg(mb_reg_dht_temperatura_001, reg[1]);
		mb.Ireg(mb_reg_dht_temperatura_002, reg[0]);
		float_to_word_array(dht.readHumidity(), reg);
		mb.Ireg(mb_reg_dht_umidade_001, reg[1]);
		mb.Ireg(mb_reg_dht_umidade_002, reg[0]);
		mb.Ireg(mb_reg_gas_001, analogRead(gas_001));
		mb.Ists(mb_reg_presenca_001, digitalRead(presenca_001));
	}
} //end loop


void float_to_word_array(float numero, word *reg)
{
	byte* array;
	array = (byte*) &numero;
	reg[0] = (array[1]<<8) | array[0];
	reg[1] = (array[3]<<8) | array[2];
} //end float_to_word_array
