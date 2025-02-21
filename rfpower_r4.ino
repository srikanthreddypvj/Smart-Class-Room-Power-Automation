#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
/////////////////////////////////

#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
#elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define INFLUXDB_URL "   "
#define INFLUXDB_TOKEN "  "
#define INFLUXDB_ORG " "
#define INFLUXDB_BUCKET "POWER Details"
#define TZ_INFO "UTC5.5"
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("wifi_status");

/////////////////////////////////
const char *ssid = " ";
const char *password = " ";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;
struct tm timeinfo;
char c[100];
int hr, mn, sec; // 24-hour format

/////////////////////////////////
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 17
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
byte sector = 0;
byte blockAddr = 2;
byte trailerBlock = 3;
byte buffer[18];
byte size = sizeof(buffer); 
byte nuidPICC[4];
MFRC522::StatusCode status;

String list[60];
String id;
int strength = 0;
int j = 0;

//////////////////////

// Relay pins for power on/off
#define light1 21
#define light2 22
int l1 = 0;
int l2 = 0;
int nos;

void setup() {
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  sntp_set_time_sync_notification_cb(timeavailable);
  esp_sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());  

  // RFID Initializations
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Relay pins for power triggering  
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  digitalWrite(light1, HIGH); // Initialize relays to off state
  digitalWrite(light2, HIGH);
}

void loop() {
  delay(1000);
  printLocalTime();
  rf();
  power();
  id.remove(0);
  cloud();
  Serial.print("Current Strength= ");
  Serial.println(strength);
  Serial.print("L1 Status= ");
  Serial.println(l1);
  Serial.print("L2 Status= ");
  Serial.println(l2);
}

void rf() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;  

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
      rfid.uid.uidByte[1] != nuidPICC[1] || 
      rfid.uid.uidByte[2] != nuidPICC[2] || 
      rfid.uid.uidByte[3] != nuidPICC[3]) {
    Serial.println(F("A card has been detected."));
    Serial.print("UID: ");
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
      id = id + String(rfid.uid.uidByte[i]);
    }
    Serial.print("Id in string is= ");
    Serial.println(id);
  } else {
    Serial.println(F("Tap only once"));
  }

  for (int s = 0; s <= 59; s++) {
    if (id == list[s]) {
      j = 1;
      return;
    } else {
      j = 0;
    }
  }
  
  if (j == 1) {
    Serial.println("You are already in class");
  } else {
    Serial.println("attendance marked");
    list[strength] = id;    
    strength++;
  }
  j = 0;

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void cloud() {
  sensor.clearFields();
  sensor.addField("rssi", WiFi.RSSI());
  sensor.addField("Strength", strength);
  sensor.addField("Light1", l1);
  sensor.addField("Light2", l2);

  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Reconnected to WiFi");
  }

  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void power() {
  if (strength == 0) {
    digitalWrite(light1, HIGH);
    digitalWrite(light2, HIGH);
    Serial.println("power off");
    l1 = 0;
    l2 = 0;
  } else if (strength >= 1 && strength <= 3) {
    digitalWrite(light1, LOW);
    digitalWrite(light2, HIGH);
    Serial.println("one light is on");
    l1 = 1;
    l2 = 0;
  } else if (strength >= 4 && strength <= 6) {
    digitalWrite(light1, LOW);
    digitalWrite(light2, LOW);
    Serial.println("two lights are on");
    l1 = 1;
    l2 = 1;
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  strftime(c, 100, "%S", &timeinfo);
  sec = timeinfo.tm_sec;
  mn = timeinfo.tm_min;
  hr = timeinfo.tm_hour;
  Serial.print(hr);
  Serial.print(" ");
  Serial.print(mn);
  Serial.print(" ");
  Serial.println(sec);
}

void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}
