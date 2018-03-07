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
const unsigned int dht_001 = 25; //testar essa porta

//<--- Modbus register offsets --->
const unsigned int mb_reg_ar_condicionado_001 = 1;
const unsigned int mb_reg_lampada_001 = 2;
const unsigned int mb_reg_dht_temperatura_001 = 3;
const unsigned int mb_reg_dht_temperatura_002 = 4;
const unsigned int mb_reg_dht_umidade_001 = 5;
const unsigned int mb_reg_dht_umidade_002 = 6;

//<--- Objetos --->
ModbusIP mb;
DHT dht(dht_001, DHT22);

//<--- Cabeçalhos de funções --->
void float_to_word_array(float, word *);


void setup()
{
	mb.config("ScadaBR", "ScadaBRunp");	//"ssid", "senha"
	dht.begin();

	//setup dos pinos
	pinMode(ar_condicionado_001, OUTPUT);
	pinMode(lampada_001, OUTPUT);
	//pinMode(dht_001, INPUT_PULLUP); //testar se isso é necessário.

	//Adicionando registradores do Modbus
	mb.addCoil(mb_reg_ar_condicionado_001);
	mb.addCoil(mb_reg_lampada_001);
	mb.addIreg(mb_reg_dht_temperatura_001);
	mb.addIreg(mb_reg_dht_umidade_001);
	mb.addIreg(mb_reg_dht_temperatura_002);
	mb.addIreg(mb_reg_dht_umidade_002);
} //end setup


void loop()
{
	mb.task();

	digitalWrite(ar_condicionado_001, !mb.Coil(mb_reg_ar_condicionado_001)); //envia 0v para acionar relé
	digitalWrite(lampada_001, !mb.Coil(mb_reg_lampada_001)); //envia 0v para acionar relé


	//necessário para converter valores float para a leitura no modbus.
	word reg[2];
	float_to_word_array(dht.readTemperature(), reg);
	mb.Ireg(mb_reg_dht_temperatura_001, reg[1]);
	mb.Ireg(mb_reg_dht_temperatura_002, reg[0]);
	float_to_word_array(dht.readHumidity(), reg);
	mb.Ireg(mb_reg_dht_umidade_001, reg[1]);
	mb.Ireg(mb_reg_dht_umidade_002, reg[0]);

	delay(200); //testar se é necessário, e trocar por millis()
} //end loop


void float_to_word_array(float numero, word *reg)
{
	byte* array;
	array = (byte*) &numero;
	reg[0] = (array[1]<<8) | array[0];
	reg[1] = (array[3]<<8) | array[2];
} //end float_to_word_array
