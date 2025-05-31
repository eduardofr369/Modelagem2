//bibliotecas necessárias

#include <IOXhop_FirebaseESP32.h>
#include <WiFiManager.h>
#include <esp_sleep.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <Adafruit_INA3221.h>


//Variaveis do Firebase

#define FIREBASE_HOST "testesdeimport-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vCXFLJtvRJ6NCF0cQC30qYppRBcU0cR17Cz3cetM"

// leds de verificação

#define ledAmarelo 33
#define ledVermelho 32
#define ledVerde 25



//variaveis do deep sleep
int long tempo_wakeup = 60; // em segundos


// objeto ads
Adafruit_ADS1115 ads;

//objeto INA

Adafruit_INA3221 ina3221;

//funções auxiliares

bool conectarWifi();
void verificarConexao();
void enviarDados();
float calcularCorrente();
float calcularTensao();
int calcularUV();
float calcularPotencia();

void setup() {
  Serial.begin(115200);

  verificarConexao(); // o código nao executa até essa função ser executada.
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Wire.begin();

  ads.begin();

  ina3221.begin(0x40, &Wire);

  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES); // quantidade de amostras 

  ina3221.setShuntResistance(1, 0.7); // valor de resistencia do shunt para o canal 2 

  ads.setGain(GAIN_TWOTHIRDS); // melhor resolução possivel para ler o vout

  esp_sleep_enable_timer_wakeup(tempo_wakeup * 1000000);

  pinMode(ledAmarelo, OUTPUT);
  


}

void loop() {
  enviarDados();

  delay(10000);

  esp_deep_sleep_start();


}

bool conectarWifi(){
  WiFiManager wm;

  bool response;

  response = wm.autoConnect("ESPGuzman");

  return response;

}

void enviarDados(){
  delay(1000);

  float dadosTensao = calcularTensao();
  float dadosCorrente = calcularCorrente();
  int dadosUV = calcularUV();
  float potencia = calcularPotencia();
  bool verificar = 1;

  Firebase.setFloat("Dados/Corrente", dadosCorrente);
  Firebase.setFloat("Dados/Tensao", dadosTensao);
  Firebase.setInt("Dados/UV", dadosUV);
  Firebase.setFloat("Dados/potencia", potencia);

  if(verificar == 1){
    digitalWrite(ledAmarelo, HIGH);
    delay(1000);
    digitalWrite(ledAmarelo, LOW);
    delay(1000);
  }

}

float calcularCorrente(){

  float corrente;

  corrente = ina3221.getCurrentAmps(1) * 1000; // conversão para mA

  return corrente;


}

void verificarConexao(){
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  while(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(ledVermelho, HIGH);
    conectarWifi();
  }

  if(WiFi.status() == WL_CONNECTED){
    digitalWrite(ledVermelho,LOW);
    digitalWrite(ledVerde, HIGH);
    delay(2000);
    digitalWrite(ledVerde, LOW);
  }
}

float calcularTensao(){

   float voltage = ina3221.getBusVoltage(1);

   return voltage;

}

int calcularUV() {
  int16_t adcValue;        // Valor lido do ADS1115 na porta A0
  float uvVoltage;         // Tensão convertida do valor ADC
  int uvIndex = -1;        // Índice UV (valor padrão inválido)

  adcValue = ads.readADC_SingleEnded(0);
  uvVoltage = ads.computeVolts(adcValue);

  if(uvVoltage < 0.05)        uvIndex = 0;
  else if(uvVoltage < 0.227)  uvIndex = 1;
  else if(uvVoltage < 0.318)  uvIndex = 2;
  else if(uvVoltage < 0.408)  uvIndex = 3;
  else if(uvVoltage < 0.503)  uvIndex = 4;
  else if(uvVoltage < 0.606)  uvIndex = 5;
  else if(uvVoltage < 0.696)  uvIndex = 6;
  else if(uvVoltage < 0.795)  uvIndex = 7;
  else if(uvVoltage < 0.881)  uvIndex = 8;
  else if(uvVoltage < 0.976)  uvIndex = 9;
  else if(uvVoltage < 1.079)  uvIndex = 10;
  else                        uvIndex = 11;

  return uvIndex;
}



}

float calcularPotencia(){
  float tensao;
  float corrente;
  float potencia;

  tensao = ina3221.getBusVoltage(1);
  corrente = corrente = ina3221.getCurrentAmps(1) * 1000;

  potencia =  tensao * corrente; // mW

  return potencia;
}
