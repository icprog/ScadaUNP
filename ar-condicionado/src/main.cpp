#include <Arduino.h>
#include <Modbus.h>
#include <ModbusIP_ESP32.h>
#include <DHT.h> //alimentar o dht11 com 3v3, e usar 10kOHM entre 3v3 e data. configurar o scada pra 5s


//pinos do controlador
//saída digital
const unsigned int ar_condicionado_001 = 33;
const unsigned int lampada_001 = 17;
//entrada digital
const unsigned int dht_001 = 5;

//modbus registers
const unsigned int mb_reg_ar_condicionado_001 = 1;
const unsigned int mb_reg_lampada_001 = 2;
const unsigned int mb_reg_dht_temperatura_001 = 3;
const unsigned int mb_reg_dht_umidade_001 = 4;

//outros
ModbusIP mb;
DHT dht(dht_001, DHT11);


void setup()
{
	mb.config("ScadaBR", "ScadaBRunp");	//"ssid", "senha"
	dht.begin();

	//setup dos pinos
		//saída digital
	pinMode(ar_condicionado_001, OUTPUT);
	pinMode(lampada_001, OUTPUT);
		//entrada digital
	pinMode(dht_001, INPUT_PULLUP);

	//setup do modbus
		//saída digital
	mb.addCoil(mb_reg_ar_condicionado_001);
	mb.addCoil(mb_reg_lampada_001);
		//entrada analógica
	mb.addIreg(mb_reg_dht_temperatura_001);
	mb.addIreg(mb_reg_dht_umidade_001);
}


void loop()
{
	mb.task();
	//saída digital
		//precisa mandar nível lógico low para a entrada do relé.
	digitalWrite(ar_condicionado_001, !mb.Coil(mb_reg_ar_condicionado_001));	//ar condicionado
	digitalWrite(lampada_001, !mb.Coil(mb_reg_lampada_001)); //lâmpada

	//entrada analógica
	mb.Ists(mb_reg_dht_temperatura_001, dht.readTemperature());
	mb.Ists(mb_reg_dht_umidade_001, dht.readHumidity());
	delay(200);
}
