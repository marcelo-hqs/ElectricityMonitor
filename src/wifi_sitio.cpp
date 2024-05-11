#include "wifi_sitio.h"

const char *wifiAuthentication[2][2] = {{"Bem-te-vi", "Marcelorj142Simonekm10Felipe"}, {"Marcelo", "marcelo2022"}};
const char *_ssid;
const char *_passphrase;

unsigned long _internetCheckInterval = 30000; // 30 segundos
unsigned long _internetLastCheck = 0;

void initWiFi(uint8_t pin)
{
  WiFi.removeEvent(WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_STA);

  size_t c = 0;
  while (true)
  {
    if (pin > 0)
    {
      digitalWrite(pin, LOW); // turn off the LED
    }

    size_t i = c % 2;
    _ssid = wifiAuthentication[i][0];
    _passphrase = wifiAuthentication[i][1];

    WiFi.begin(_ssid, _passphrase);
    delay(100);

    bool turnOnLed = true;
    while (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL)
    {
      if (pin > 0)
      {
        digitalWrite(pin, turnOnLed ? HIGH : LOW); // blink the LED
      }
      delay(1000);
      turnOnLed = !turnOnLed;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }

    c++;
  }

  if (pin > 0)
  {
    digitalWrite(pin, HIGH); // turn on the LED
  }
  _internetLastCheck = millis();
}

void checkWiFi()
{
  unsigned long currentMillis = millis();
  if (currentMillis - _internetLastCheck > _internetCheckInterval)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.reconnect();
      _internetLastCheck = currentMillis;
    }
  }
}