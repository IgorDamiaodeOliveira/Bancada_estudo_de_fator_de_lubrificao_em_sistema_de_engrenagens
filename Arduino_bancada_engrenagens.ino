#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif


// --- CONFIGURAÇÃO DS18B20 ---
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

// --- CONFIGURAÇÃO LCD 16x2 ---
LiquidCrystal lcd(8,9,10,11,12,13); // Pinos comuns para LCD 16x2.

// --- CONFIGURAÇÃO HX711 (CÉLULA DE CARGA) ---
const int HX711_dout = 7; 
const int HX711_sck = 6; 

HX711_ADC LoadCell(HX711_dout, HX711_sck);

// CRÍTICO: SUBSTITUA O 0.0 PELO SEU VALOR DE CALIBRAÇÃO!
float calibrationValue = 1720;

unsigned long t = 0; 
const int serialPrintInterval = 500;


void setup(void)
{
  Serial.begin(57600);
  delay(100);
  Serial.println("Sistema Integrado - Temp, Peso e LCD");

  // --- SETUP DS18B20 ---
  sensor.begin();

  // --- SETUP LCD ---
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Bancada Dion JR.");
  lcd.setCursor(0,1);
  lcd.print("Estudo Rolamentos");
  delay(1000);
  lcd.clear();


  // --- SETUP HX711 (CÉLULA DE CARGA) ---
  LoadCell.begin();

  unsigned long stabilizingtime = 2000;
  boolean _tare = true;

  Serial.println("Iniciando HX711...");
  LoadCell.start(stabilizingtime, _tare);

  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout HX711. Verifique a fiacao!");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); 
    Serial.println("Inicio de Operacao Completo.");
    Serial.print("Fator de Calibracao: ");
    Serial.println(calibrationValue);
  }
}

void loop(void)
{
  // 1. VERIFICA SE HÁ NOVOS DADOS DO HX711 E INICIA A PRÓXIMA CONVERSÃO (DEVE RODAR RÁPIDO!)
  if (LoadCell.update()); 

  // 2. EXIBIR DADOS SOMENTE NO INTERVALO DE TEMPO DEFINIDO (A CADA 1000ms)
  if (millis() > t + serialPrintInterval) {
    
    // Leitura da temperatura e peso
    sensor.requestTemperatures();
    float temperatura = sensor.getTempCByIndex(0);
    float peso = LoadCell.getData(); 

    // **SERIAL MONITOR**
    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.print(" C | Peso: ");
    Serial.print(peso, 2);
    Serial.println(" g");

    // **EXIBIÇÃO NO LCD**
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(temperatura, 1); 
    lcd.print((char)223); 
    lcd.print("C       "); 

    lcd.setCursor(0,1);
    lcd.print("Peso: ");
    lcd.print(peso, 2); 
    lcd.print(" g      "); 

    t = millis(); 
  }
  
  // 3. CONTROLE DE TARE (ZERAGEM) VIA SERIAL
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  if (LoadCell.getTareStatus() == true) {
    Serial.println("Zeragem Completa (Tare complete)");
  }
  
  // LINHA REMOVIDA/CORRIGIDA: Não é recomendado usar delay() aqui.
}