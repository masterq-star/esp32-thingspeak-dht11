#include "DHT.h"
#include "WiFi.h"
#include <ThingSpeak.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const char *ssid =  "HTPro-T1";
const char *pass =  "Ht88888888";
const char *server = "api.thingspeak.com";
unsigned long myChannelNumber =1251955;
const char * myWriteAPIKey = "BJ0SJ1C4MNOG8NJE";
int lastHumi , lastTemp;
unsigned long send_time = 0, dht_time = 0;
unsigned long dht_time_repeat = 3000, send_time_repeat = 15000;
float h = 0;
float t = 0;
WiFiClient client;
void setup() {
  Serial.begin(115200);
  Serial.println("thingspeak-lap");
  dht.begin();
  setupWiFi();
}
void loop() {
  //  delay(2000);
  if (millis() - dht_time >= dht_time_repeat) {
    readDHT11();
    dht_time = millis();
  }
  if (millis() - send_time >= send_time_repeat) {
    client_Sendata();
    send_time = millis();
  }
  //  client_Sendata();
  //  delay(20000);
}
void client_Sendata() {
  static boolean data_state = false;
  if ( data_state )
  {
    ThingSpeak.writeField(myChannelNumber, 1, lastTemp, myWriteAPIKey);
    data_state = false;
  }
  else
  {
    ThingSpeak.writeField(myChannelNumber, 2, lastHumi, myWriteAPIKey);
    data_state = true;
  }
  Serial.println("Data send to ThingSpeak");
}
void setupWiFi() {
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}
void readDHT11() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    lastHumi = h;
    lastTemp = t;
  }
  Serial.print("Humidity: ");
  Serial.print(lastHumi);
  Serial.print("%t");
  Serial.print("Temperature: ");
  Serial.print(lastTemp);
  Serial.print("°C");
  Serial.println();
}
