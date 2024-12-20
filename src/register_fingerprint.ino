#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <UbidotsESPMQTT.h>

#define UBIDOTS_TOKEN "BBUS-2NwvOUMugx40weHH5AbJyNK8X7MKLL" 
#define DEVICE_LABEL "fingerprint_device"
#define VARIABLE_LABEL "fingerprint_id"

const char* ssid = "casa";
const char* password = "asw21kl.912";

uint8_t id;

uint8_t getFingerprintEnroll();

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Ubidots ubidots(UBIDOTS_TOKEN);

void setup() {
  Serial.begin(9600);
  Serial.println("Procurando Leitor Biometrico ...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi!");

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Leitor Biometrico encontrado");
  } else {
    Serial.println("Leitor Biometrico nao encontrado");
    while (1);
  }

  ubidots.begin();
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  boolean validnum = false;
  while (1) {
    while (!Serial.available());
    char c = Serial.read();
    if (isdigit(c)) {
      num *= 10;
      num += c - '0';
      validnum = true;
    } else if (validnum) {
      return num;
    }
  }
}

void loop() {
  Serial.println("Pronto para Cadastrar Dados! Indique o ID# o qual deve ser salvo");
  id = readnumber();
  Serial.print("Inscrever ID#");
  Serial.println(id);

  while (!getFingerprintEnroll());

  ubidots.add(VARIABLE_LABEL, id);
  if (ubidots.send(DEVICE_LABEL)) {
    Serial.println("ID enviado para Ubidots com sucesso!");
  } else {
    Serial.println("Falha ao enviar ID para Ubidots.");
  }
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Esperando digital para inscrever #");
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagem Capturada");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erro ao se Comunicar");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Erro ao Capturar Imagem");
        break;
      default:
        Serial.println("Erro Desconhecido");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem muito Confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro ao se Comunicar");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Impossivel encontrar caracteristicas da digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Impossível encontrar características da digital");
      return p;
    default:
      Serial.println("Erro Desconhecido");
      return p;
  }

  Serial.println("Retire o dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Coloque o mesmo dedo novamente");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagem Capturada");
 break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erro ao se Comunicar");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Erro ao Capturar Imagem");
        break;
      default:
        Serial.println("Erro Desconhecido");
        break;
    }
  }

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem Convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem muito Confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Nao foi possível encontrar características da impressao digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Nao foi possível encontrar características da impressao digital");
      return p;
    default:
      Serial.println("Erro Desconhecido");
      return p;
  }

  Serial.print("Criando cadastro para #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Digital Combinada");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro ao se Comunicar");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Digital nao corresponde");
    return p;
  } else {
    Serial.println("Erro Desconhecido");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Armazenado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro ao se Comunicar");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Impossível Armazenar Dados");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erro ao salvar na memoria");
    return p;
  } else {
    Serial.println("Erro Desconhecido");
    return p;
  }
}
