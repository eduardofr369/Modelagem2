//bibliotecas necessárias

#include <IOXhop_FirebaseESP32.h>
#include <WiFiManager.h>


//Variaveis do Firebase

#define FIREBASE_HOST "testesdeimport-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "vCXFLJtvRJ6NCF0cQC30qYppRBcU0cR17Cz3cetM"



//funções auxiliares

bool conectarWifi();
void enviarDados();




void setup() {
  Serial.begin(115200);

  conectarWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  enviarDados();


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

  int dadosTensao = 1;
  int dadosCorrente = 2;
  int dadosUV = 3;

  Firebase.setInt("Dados/Tensao", dadosTensao);
  Firebase.setInt("Dados/Corrente", dadosCorrente);
  Firebase.setInt("Dados/UV", dadosUV);


}
