# Lixeira Inteligente Seletiva (ESP32 + Firebase)

Este repositório contém o firmware para uma lixeira inteligente que separa resíduos (recicláveis e orgânicos) automaticamente. O sistema é controlado por um ESP32 e recebe comandos de um sistema externo (como uma IA de visão computacional) através do Firebase Realtime Database.

Este projeto demonstra a integração de hardware (servo motor) com um banco de dados em nuvem (Firebase) para controle em tempo real.

## ✨ Funcionalidades

* **Separação Automática:** O ESP32 "ouve" um nó `/comando` no Firebase.
* **Comandos:** Aceita o comando "reciclavel" (move o servo para 0°) ou "organico" (move o servo para 180°).
* **Sistema de Pontos:** Incrementa um nó `/pontos` no Firebase a cada descarte correto, permitindo a gamificação.
* **Comando de Admin:** Permite zerar os pontos enviando o texto "zerar pontos" pela Serial Monitor do Arduino.
* **Segurança:** Utiliza um arquivo `config.h` (ignorado pelo Git) para armazenar credenciais de Wi-Fi e Firebase.

## 🛠️ Hardware Necessário

* ESP32
* Servo Motor (ex: SG90 ou MG996R)

## 📦 Bibliotecas (Arduino/PlatformIO)

* `WiFi.h`
* `Firebase_ESP_Client.h`
* `ESP32Servo.h`

## 🚀 Configuração

1.  Clone este repositório.
2.  Crie uma cópia do arquivo `config.example.h` e renomeie-a para `config.h`.
3.  Preencha o `config.h` com suas credenciais de Wi-Fi e Firebase:

    ```cpp
    // Arquivo: config.h
    #ifndef CONFIG_H
    #define CONFIG_H

    #define WIFI_SSID "SuaRedeWifi"
    #define WIFI_PASSWORD "SuaSenhaWifi"

    #define FIREBASE_HOST "[https://seu-projeto.firebaseio.com](https://seu-projeto.firebaseio.com)"
    #define FIREBASE_AUTH "SuaChaveSecretaDoFirebase" // Legacy Token

    #endif
    ```
4.  **Importante:** A chave `FIREBASE_AUTH` é o **Segredo do Banco de Dados** (legacy token), encontrado nas Configurações do Projeto > Contas de Serviço > Segredos do Banco de Dados.
5.  Compile e envie o código para o seu ESP32.

## 🌲 Estrutura do Firebase

Este firmware espera a seguinte estrutura no seu Firebase Realtime Database:

```json
{
  "comando": "clear",
  "pontos": 0
}
