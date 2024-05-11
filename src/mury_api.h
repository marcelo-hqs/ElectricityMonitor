#ifndef MURY_API_H /* include guards */
#define MURY_API_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String getToken();

bool api_put(String reqst, String token, JsonDocument doc);

bool api_post(String reqst, String token, JsonDocument doc);

void pingClient(String token);

#endif /* MURY_API_H */