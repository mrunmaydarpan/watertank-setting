#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#define debug_state false

#if debug_state
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

const char *min_ = "min";
const char *max_ = "max";
const char *thres_ = "threshold";
const char *stator_ = "stator";
int get_min(0), get_max(0), get_threshold(0), get_stator(0);
String Command;
const byte DNS_PORT = 53;

AsyncWebServer server(80);
DNSServer dns;
WiFiClient client;
extern const char index_html[];

String processor(const String &var)
{
  if (var == "setmin")
  {
    String sendValue = "";
    sendValue += "<input type=\"number\" name=\"min\" maxlength=\"2\" value=" + String(get_min) + " > ";
    return sendValue;
  }
  if (var == "setmax")
  {
    String sendValue = "";
    sendValue += "<input type=\"number\" name=\"max\" maxlength=\"3\" value=" + String(get_max) + "> ";
    return sendValue;
  }
  if (var == "setthreshold")
  {
    String sendValue = "";
    sendValue += "<input type=\"number\" name=\"threshold\" maxlength=\"2\" value=" + String(get_threshold) + "> ";
    return sendValue;
  }
  if (var == "setstator")
  {
    String sendValue = "";
    sendValue += "<input type=\"number\" name=\"stator\" maxlength=\"1\" value=" + String(get_stator) + " > ";
    return sendValue;
  }
  return String();
}

void parseCommand(String com)
{
  if (com.startsWith("$"))
  {
    String part1 = com.substring(0, com.indexOf(":"));
    String part2 = com.substring(com.indexOf(":") + 1);

    if (part1.equals("$minD"))
    {
      int i = part2.toInt();
      get_min = i;
      debugln("get min");
    }
    if (part1.equals("$maxD"))
    {
      int i = part2.toInt();
      get_max = i;
      debugln("get max");
    }
    if (part1.equals("$startAt"))
    {
      int i = part2.toInt();
      get_threshold = i;
      debugln("get threshold");
    }
    if (part1.equals("$type"))
    {
      int i = part2.toInt();
      get_stator = i;
      debug("get stator");
    }
  }
}

void getData()
{
  if (Serial.available())
  {
    char data = Serial.read();
    if (data == '\n')
    {
      parseCommand(Command);
      Command = "";
    }
    else
    {
      Command += data;
    }
  }
}

void setting_code()
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    {
                      Serial.println();
                      request->send_P(200, "text/html", index_html, processor);
                      Serial.println("$readSetting:1");
                    });
  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              int min;
              int max;
              int threshold;
              int stator;
              String message;
              if (request->hasParam(min_) && request->hasParam(max_) && request->hasParam(thres_) && request->hasParam(stator_))
              {
                min = request->getParam(min_)->value().toInt();
                max = request->getParam(max_)->value().toInt();
                threshold = request->getParam(thres_)->value().toInt();
                stator = request->getParam(stator_)->value().toInt();
                if (min == 0 || max == 0 || threshold == 0)
                {
                  message = "invalid values (\"0\") are not allowed";
                }
                else if (min < max && threshold <= 70 && threshold >= 20 && stator < 4 && stator != 0)
                {
                  delay(50);
                  Serial.printf("$minD:%d\n", min ? min : get_min);
                  delay(50);
                  Serial.printf("$maxD:%d\n", max ? max : get_max);
                  delay(50);
                  Serial.printf("$startAt:%d\n", threshold ? threshold : get_threshold);
                  delay(50);
                  Serial.printf("$type:%d\n", stator ? stator : get_stator);
                  delay(50);
                  message = "min: ";
                  message += String(min) + '\n';
                  message += "max: ";
                  message += String(max) + '\n';
                  message += "start at: ";
                  message += String(threshold) + '\n';
                  message += "stator: ";
                  message += String(stator);
                }
              }
              else
              {
                message = "No message sent";
              }
              Serial.println("$readSetting:1");
              request->send(200, "text/plain", message);
            });
  // server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  //             Serial.println("$readSetting:1");
  //             char data[32];
  //             sprintf(data, "min: %d\nmax: %d\nstart at: %d\n", get_min, get_max, get_threshold);
  //             request->send(200, "text/plain", String(data));
  //           });
  dns.start(DNS_PORT, "*", IPAddress(WiFi.softAPIP()));
  server.begin();
}

void setup()
{
  delay(3000);
  Serial.begin(9600);
  String ssid = "WaterTank-Setting-";
  ssid += String(ESP.getChipId()).c_str();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, "12345678");
  debug("AP IP: ");
  debugln(WiFi.softAPIP());
  setting_code();
  Serial.println("$readSetting:1");
}

void loop()
{
  dns.processNextRequest();
  getData();
}