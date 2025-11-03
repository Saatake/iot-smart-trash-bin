#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Servo.h>

#include "config.h" 

FirebaseData fbdo;
FirebaseConfig config;

Servo servo;

int servoPin = 4;
int servoInicial = 90; // Posição de repouso
int pontos = 0; // Cache local dos pontos

void moverServo(int destino);
void incrementarPontos(int valor);
void zerarPontos();
void conectarWiFi();


void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Conectando ao Wi-Fi...");
  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED && tentativas < 5) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi conectado!");
  } else {
    Serial.println("\nFalha ao conectar. Funcionalidade online desabilitada.");
  }
}


void setup() {
  Serial.begin(115200);

  conectarWiFi();

  servo.attach(servoPin);
  servo.write(servoInicial);

  if (WiFi.status() == WL_CONNECTED) {
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.begin(&config, nullptr);
    Firebase.reconnectWiFi(true);

    // Carrega os pontos iniciais
    if (Firebase.RTDB.getInt(&fbdo, "/pontos")) {
      pontos = fbdo.intData();
      Serial.println("Pontos iniciais carregados: " + String(pontos));
    } else {
      Serial.println("Erro ao carregar pontos: " + fbdo.errorReason());
    }
  }
}

void loop() {
  static String ultimoComando = "";

  if (WiFi.status() == WL_CONNECTED) {
    
    if (Firebase.RTDB.getString(&fbdo, "/comando")) {
      String comando = fbdo.stringData();

      // Processa apenas comandos novos
      if (comando != ultimoComando && comando != "clear" && comando != "") {
        ultimoComando = comando; 
        Serial.println("Comando recebido: " + comando);

        if (comando == "reciclavel") {
          moverServo(0);
          incrementarPontos(10);
        } else if (comando == "organico") {
          moverServo(180);
          incrementarPontos(10);
        }

        // Limpa a flag no Firebase
        if (!Firebase.RTDB.setString(&fbdo, "/comando", "clear")) {
          Serial.println("Erro ao limpar comando: " + fbdo.errorReason());
        }
      }
    }
  }

  // Comando local de admin
  if (Serial.available()) {
    String entradaSerial = Serial.readString();
    entradaSerial.trim();

    if (entradaSerial == "zerar pontos") {
      if (WiFi.status() == WL_CONNECTED) {
        zerarPontos();
      } else {
        Serial.println("Erro: Sem Wi-Fi.");
      }
    }
  }

  delay(1000); // Poll rate
}

void moverServo(int destino) {
  int posicaoAtual = servo.read();

  if (destino > posicaoAtual) {
    for (int pos = posicaoAtual; pos <= destino; pos++) {
      servo.write(pos);
      delay(20);
    }
  } else {
    for (int pos = posicaoAtual; pos >= destino; pos--) {
      servo.write(pos);
      delay(20);
    }
  }

  delay(5000); // Pausa com a tampa aberta
  servo.write(servoInicial); // Retorna ao repouso
}

void incrementarPontos(int valor) {
  pontos += valor;
  if (!Firebase.RTDB.setInt(&fbdo, "/pontos", pontos)) {
    Serial.println("Erro ao atualizar pontos: " + fbdo.errorReason());
  } else {
    Serial.println("Pontos atualizados: " + String(pontos));
  }
}

void zerarPontos() {
  pontos = 0;
  if (!Firebase.RTDB.setInt(&fbdo, "/pontos", pontos)) {
    Serial.println("Erro ao zerar pontos: " + fbdo.errorReason());
  } else {
    Serial.println("Pontos zerados.");
  }
}
