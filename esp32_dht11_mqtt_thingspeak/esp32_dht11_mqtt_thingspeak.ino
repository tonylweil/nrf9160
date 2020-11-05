

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2                           // DHT Sensor connected to digital pin 2.
#define DHTTYPE DHT11                      // Type of DHT sensor.
DHT dht(DHTPIN, DHTTYPE);  


char ssid[] = "xxxxx";          //  Change this to your network SSID (name).
char pass[] = "xxxxx";           // Change this to your network password.
char mqttUserName[] = "TSArduinoMQTTDemo";  // Use any name.
char mqttPass[] = "xxxxxxxxxx";      // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "xxxxxxxxxx";    // Change to your channel write API key.
long channelID = 769080;                    // Change to your channel ID.

static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.


WiFiClient client;                           // Initialize the Wi-Fi client library.

PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com"; 

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds.


// Define the mqttPublishFeed method to publish the sensor data to a ThingSpeak channel feed. You can publish to multiple fields at once using this function. 
// This example publishes to fields 1, 2, and 3 of the channel.





void setup() {
  
  Serial.begin(115200);
  int status = WL_IDLE_STATUS;  // Set temporary Wi-Fi status.

  // Attempt to connect to Wi-Fi network.
  while (status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.

  dht.begin();
}

void loop() {

 // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected()) 
  {
  reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval) 
  {
    mqttPublishFeed(); // Publish three values simultaneously.
    // mqttPublishField(fieldNumber); // Use this function to publish to a single fi
  } 
  
}

void mqttPublishFeed() {
  
 // Wait a few seconds between measurements.
  delay(2000);
  
  float f = dht.readTemperature(true); // Read temperature from DHT sensor.
  float h = dht.readHumidity();  // Read humidity from DHT sensor.
  //int lightLevel = analogRead(LIGHTPIN); // Read voltage from light sensor.
  int x = 99;

  Serial.print("Temperature: ");
  Serial.print(f,1);
  Serial.print("°F");
  Serial.print("  ");
  Serial.print("Humidity: ");
  Serial.print(h,1);
  Serial.println("%");
  
  // Create data string to send to ThingSpeak.
  String data = String("field1=") + String(f, 1) + "&field2=" + String(h, 1) + "&field3=" + String(x, 1);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer=data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString = "channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  mqttClient.publish( topicBuffer, msgBuffer );
  lastConnectionTime = millis();
}

  void reconnect() 
  {
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
