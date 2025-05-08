//bibliotecas necessárias

#include <IOXhop_FirebaseESP32.h>
#include <WiFiManager.h>
#include <esp_sleep.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>


//Variaveis do Firebase

#define FIREBASE_HOST "testesdeimport-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vCXFLJtvRJ6NCF0cQC30qYppRBcU0cR17Cz3cetM"


//variaveis do deep sleep
int long tempo_wakeup = 60; // em segundos


// objeto ads
Adafruit_ADS1115 ads;


//funções auxiliares

bool conectarWifi();
void enviarDados();
float calcularCorrente();
float calcularTensao();
float calcularUV();


void setup() {
  Serial.begin(115200);
 
  conectarWifi();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  ads.begin();

  Wire.begin();
 
  esp_sleep_enable_timer_wakeup(tempo_wakeup * 1000000);

  ads.setGain(GAIN_TWOTHIRDS); // melhor resolução possivel para ler o vout


}

void loop() {
  enviarDados();

  //delay(30000);

  esp_deep_sleep_start();


}

bool conectarWifi(){
  WiFiManager wm;

  bool response;

  response = wm.autoConnect("ESPGuzman");

  if(!response){
    Serial.print("Wifi não funcionou seu lixo");
  }
  else{
    Serial.print("Conectado!");
  }

  return response;

}

void enviarDados(){

  float dadosTensao = 1;
  float dadosCorrente = calcularCorrente();
  float dadosUV = 3;

  Firebase.setInt("Dados/Tensao", dadosTensao);
  Firebase.setFloat("Dados/Corrente", dadosCorrente);
  Firebase.setInt("Dados/UV", dadosUV);


}

float calcularCorrente(){
  const float sensibilidade = 0.185;
  float correnteAcumulada = 0;
  int16_t adc0;
  float out;

  const int amostras = 10000;

  for (int i = 0; i < amostras; i++) {
    adc0 = ads.readADC_SingleEnded(0);

    out = ads.computeVolts(adc0);

    correnteAcumulada += (out - 2.5) / sensibilidade;

    delay(1);
  }

  float corrente = correnteAcumulada / amostras;

  return corrente;
}
