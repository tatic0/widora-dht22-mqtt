#include "Arduino.h"
#include "WiFi.h"
#include "DHT.h"
#include "password.h"
#include "PubSubClient.h"
//#include "esp_deep_sleep.h"
#include "esp_sleep.h"

#define DHTPIN  0       // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

char tmp[11];
char hum[11];

void DHT_get_data() {
    float h = dht.readHumidity();
    delay(300);
    float t = dht.readTemperature();
    delay(300);
		Serial.println(h*1.00);
		Serial.println(t*1.00);
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
      client.setServer("192.168.1.7", 1883);
      client.connect("widora1");
			Serial.println("reconnecting...");
			Serial.print(".");
      //delay(1000);
      return;
    } else {
    	client.setServer("192.168.1.7", 1883);
    	client.connect("widora1");
			Serial.println("connected, pushing data");
      client.publish("sensor/hum1", (char*) hum.c_str());
			Serial.println(hum.c_str());
			client.disconnect();
			delay(1000);
    	client.setServer("192.168.1.7", 1883);
    	client.connect("widora1");
			Serial.println("connected, pushing data");
      client.publish("sensor/tmp1", (char*) tmp.c_str());
			Serial.println(tmp.c_str());
			client.disconnect();
			Serial.println("going to sleep");
      //esp_deep_sleep(15000000);
      esp_deep_sleep(1800000000);
   } 
}


void setup() {
    Serial.begin(9600);
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
		char WS = WiFi.status();
    Serial.println(WS);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.print(WS);
    }   
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Setup done");
}

void loop() {
  DHT_get_data(); 
}
