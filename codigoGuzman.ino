//bibliotecas necessárias

#include <IOXhop_FirebaseESP32.h>
#include <WiFiManager.h>
#include <esp_sleep.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>


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


//funções auxiliares

bool conectarWifi();
bool enviarDados();
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

  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);


}

void loop() {
  enviarDados();

  if(enviarDados() == 1){
    digitalWrite(ledAmarelo, HIGH);
  }

  delay(10000);

  


  esp_deep_sleep_start();


}

bool conectarWifi(){
  WiFiManager wm;

  bool response;

  response = wm.autoConnect("ESPGuzman");

  if(!response){
    digitalWrite(ledVermelho, HIGH);
  }
  else{
    digitalWrite(ledVerde, HIGH);
    delay(3000);
    digitalWrite(ledVerde, LOW);
  }

  return response;

}

bool enviarDados(){

  float dadosTensao = 1;
  float dadosCorrente = 2;
  float dadosUV = 3;
  bool verificar;

  Firebase.setInt("Dados/Tensao", dadosTensao);
  Firebase.setFloat("Dados/Corrente", dadosCorrente);
  Firebase.setInt("Dados/UV", dadosUV);

  verificar = 1;

  return verificar;

}

float calcularCorrente(){
  const float sensibilidade = 0.185;
  float correnteAcumulada = 0;
  int16_t adc0;
  float out;

  const int amostras = 10;

  for (int i = 0; i < amostras; i++) {
    adc0 = ads.readADC_SingleEnded(0);

    out = ads.computeVolts(adc0);

    correnteAcumulada += (out - 2.5) / sensibilidade;

    delay(1);
  }

  float corrente = correnteAcumulada / amostras;

  return corrente;
}
