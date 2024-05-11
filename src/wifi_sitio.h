#ifndef WIFI_SITIO_H /* include guards */
#define WIFI_SITIO_H

#include <Arduino.h>
#include <WiFi.h>

void initWiFi(uint8_t pin = 0);

void checkWiFi();

#endif /* WIFI_SITIO_H */