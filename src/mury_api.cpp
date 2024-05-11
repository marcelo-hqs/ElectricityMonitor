#include "mury_api.h"

#define REST_API_SERVER "https://mury.hq.dev.br/index.php/"

String getToken()
{
  String token = "";

  HTTPClient http;
  String reqst = "https://mury.hq.dev.br/login.php";
  http.begin(reqst.c_str());
  http.addHeader("Content-Type", "application/json");

  // Prepare JSON document
  JsonDocument doc;
  doc["login"] = "climamury";
  doc["password"] = "Onh78c5g4tYjh";

  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  int httpCode = http.POST(json);
  if (httpCode == 200)
  {
    String payload = http.getString();

    char jsonResp[1024];
    payload.toCharArray(jsonResp, 1024);

    JsonDocument tokenInfo;
    deserializeJson(tokenInfo, jsonResp);

    const char *jwt = tokenInfo["jwt"];

    token += "MURY_TOKEN=";
    token += jwt;
  }

  http.end();

  return token;
}

bool api_put(String reqst, String token, JsonDocument doc)
{
  bool rc = true;

  HTTPClient http;
  String url = REST_API_SERVER + reqst + token;

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  int httpCode = http.PUT(json);
  if (httpCode >= 0)
  {
    // lê retorno apenas para evitar exception
    String s = http.getString();
  }
  else
  {
    rc = false;
  }

  http.end();

  return rc;
}

bool api_post(String reqst, String token, JsonDocument doc)
{
  bool rc = true;

  HTTPClient http;
  String url = REST_API_SERVER + reqst + token;

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  int httpCode = http.POST(json);
  if (httpCode >= 0)
  {
    // lê retorno apenas para evitar exception
    String s = http.getString();
  }
  else
  {
    rc = false;
    // Serial.println(String("[HTTP] POST... failed, error: ") + http.errorToString(httpCode));
  }

  http.end();

  return rc;
}

void pingClient(String token)
{
  if (token.length() == 0)
  {
    return;
  }

  // Prepare JSON document
  JsonDocument doc;
  doc["id"] = 10;

  api_put("client/ping?", token, doc);
}
