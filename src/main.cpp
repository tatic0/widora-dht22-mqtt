#include "Arduino.h"
#include "WiFi.h"
#include "DHT.h"
#include "password.h"
#include "PubSubClient.h"
#include "esp_deep_sleep.h"

#define DHTPIN  0       // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
// sensor/hum
// sensor/tmp
char tmp[11];
char hum[11];

void DHT_get_data() {
    float h = dht.readHumidity();
    delay(300);
    float t = dht.readTemperature();
    delay(300);
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      delay(300);
      return;
    }
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println();
    Serial.println(WiFi.localIP());
    String tmp = String(t);
    String hum = String(h);
    if (!client.connected()){
      client.connect("widora1");
      delay(1000);
      return;
    } else {
      client.publish("sensor/hum", (char*) hum.c_str());
      client.publish("sensor/tmp", (char*) tmp.c_str());
      client.disconnect();
      esp_deep_sleep(1500000000);
      //esp_deep_sleep(15000000);
    }
}


void setup()
{
    Serial.begin(9600);
    Serial.print(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }   
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Setup done");
    client.setServer("bike.dragon-tortuga.net", 1883);
    client.connect("widora1");
    client.disconnect();
}

void loop() {
  DHT_get_data(); 
}
