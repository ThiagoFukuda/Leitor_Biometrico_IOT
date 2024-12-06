#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <UbidotsESPMQTT.h>

#define UBIDOTS_TOKEN "BBUS-2NwvOUMugx40weHH5AbJyNK8X7MKLL" 
#define DEVICE_LABEL "fingerprint_device"
#define VARIABLE_LABEL "fingerprint_id"

const char* ssid = "casa";
const char* password = "asw21kl.912";

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const int ledPin = 10;

void setup()  {  
  Serial.begin(9600);
  Serial.println("Iniciando Leitor Biometrico");

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Leitor Biometrico Encontrado");
  } else {
    Serial.println("Leitor Biometrico nao encontrado");
    while (1);
  }
  
  pinMode(ledPin, OUTPUT);
  Serial.println("Esperando Dedo para Verificar");
}

void loop() {
  int id = getFingerprintIDez();
  
  if (id >= 0) {
    Serial.print("Dedo reconhecido! ID: ");
    Serial.println(id);
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }
  
  delay(850);
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  
  if (p == FINGERPRINT_NOFINGER) {
    return -1;
  }

  if (p != FINGERPRINT_OK) {
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    return finger.fingerID; 
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Dedo nao reconhecido.");
    return -1;
  } else {
    return -1;
  }
}
