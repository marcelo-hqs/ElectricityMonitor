#include "wifi_sitio.h"
#include "mury_api.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <ZMPT101B.h>

#define TENSAOPIN 36

#undef ARDUINO_ARCH_ESP32
#define ARDUINO_ARCH_ESP32 1

ZMPT101B voltageSensor(TENSAOPIN);

unsigned long _clientCheckInterval = 15 * 60 * 1000; // 15 minutos p/ milisegundos
unsigned long _clientLastPingElectricity = 0;
float _tensaoAcc = 0.0;
int _tensaoCnt = 0;

String _token = "";

void electricityClient()
{
  if (_token.length() == 0 || _tensaoCnt == 0)
  {
    return;
  }

  float tensao = _tensaoAcc / _tensaoCnt;

  Serial.println(String("Electricity..."));

  // Prepare JSON document
  JsonDocument doc;
  doc["id"] = 10;
  doc["status"] = tensao;

  api_put("client/electricity?", _token, doc);

  _tensaoAcc = 0.0;
  _tensaoCnt = 0;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("iniciando setup...");

  initWiFi();

  voltageSensor.setZeroPoint(2866);
  voltageSensor.setSensitivity(0.010000);

  Serial.println("setup OK.");
}

void loop()
{
  checkWiFi();

  float tensao = voltageSensor.getVoltageAC(60);
  Serial.println("Tensao: " + String(tensao) + "V");

  if (tensao > 0)
  {
    _tensaoAcc += tensao;
    _tensaoCnt++;
  }

  unsigned long currentMillis = millis();
  // ping e electricity de 15 em 15 minutos
  if (tensao < 100.0 || _clientLastPingElectricity == 0 || ((currentMillis - _clientLastPingElectricity) > _clientCheckInterval))
  {
    _token = getToken();
    pingClient(_token);
    electricityClient();
    _clientLastPingElectricity = currentMillis;
    _token = "";
  }

  delay(3000);
}