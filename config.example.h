// Arquivo: config.example.h
//
// Este é um arquivo de exemplo. Copie este arquivo,
// renomeie a cópia para "config.h" e preencha
// com suas credenciais reais.
//

#ifndef CONFIG_H
#define CONFIG_H

// --- CONFIG WIFI ---
// Coloque aqui o SSID (nome) e a senha da sua rede Wi-Fi
#define WIFI_SSID "ColoqueSeuWifiAqui"
#define WIFI_PASSWORD "ColoqueSuaSenhaAqui"

// --- CONFIG FIREBASE ---
// URL do seu Realtime Database
#define FIREBASE_HOST "https://seu-projeto.firebaseio.com"

// Chave Secreta do Banco de Dados (Legacy Token)
// Encontrada em: Configurações do Projeto > Contas de Serviço > Segredos do Banco de Dados
#define FIREBASE_AUTH "SuaChaveSecretaDoFirebaseAqui"

#endif
