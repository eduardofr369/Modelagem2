//bibliotecas necessárias

#include <IOXhop_FirebaseESP32.h>
#include <WiFiManager.h>
#include <esp_sleep.h>


//Variaveis do Firebase

#define FIREBASE_HOST "testesdeimport-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vCXFLJtvRJ6NCF0cQC30qYppRBcU0cR17Cz3cetM"


//variaveis do deep sleep
int long tempo_wakeup = 60; // em segundos



//funções auxiliares

bool conectarWifi();
void enviarDados();
float calcularCorrente();


void setup() {
  Serial.begin(115200);
 
  conectarWifi();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
  esp_sleep_enable_timer_wakeup(tempo_wakeup * 1000000);


}

void loop() {
  enviarDados();

  delay(30000);

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
  Firebase.setInt("Dados/Corrente", dadosCorrente);
  Firebase.setInt("Dados/UV", dadosUV);


}

float calcularCorrente(){
// vamos supor aqui que o algoritmo para calcular a corrente seja baseado na lei de ohm
// essa função aqui é só para demonstrar como podemos guardar o valor de uma função dentro de uma variavel
// a partir disso vamos enviar para o firebase


float resistencia = 20; 
float tensao = 200;
float corrente = tensao / resistencia; 


return corrente;

}
