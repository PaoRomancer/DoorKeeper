#include <WiFi.h>
#include <PubSubClient.h>

#include <SPI.h>

#include <MFRC522.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 7     // Define the pin for the LED
#define BUTTON_PIN 3  // Define the pin for the button
#define BUZZER_PIN 6  // Define the pin for the buzzer
#define RELAY_PIN 2   // Define the pin for the relay
#define SENSOR_PIN A1
#define NOTIFY_VPIN V3
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "fuxk";                      //อย่าลืมแก้ไข SSID ของ WIFI ที่จะให้ NodeMCU ไปเชื่อมต่อ
const char* password = "12345678";              //อย่าลืมแก้ไข PASSWORD ของ WIFI
const char* mqtt_server = "broker.hivemq.com";  //อย่าลืมแก้ไข BROKER
const int mqtt_port = 1883;
const char* mqtt_Client = "fa54c3d5-205c-4eaa-8695-xxxxxxx";  //อย่าลืมแก้ไข ClientID
const char* mqtt_username = "";                               //อย่าลืมแก้ไข Token
const char* mqtt_password = "";                               //อย่าลืมแก้ไข Secret
int totalCount = 0;                                           // ตัวแปรสำหรับนับจำนวนรวมที่ผ่าน IR Sensor ทั้งหมด
int newScanCount = 0;                                         // ตัวแปรสำหรับนับจำนวนของแต่ละบัตรสแกนใหม่
int count = 0;                                                // Variable to hold the count
int sensorValue = 0;                                          // Variable to hold the sensor reading
int threshold = 500;                                          // Define a threshold value (adjust based on testing)
unsigned long entryStartTime = 0;
unsigned long exitStartTime = 0;
long lastMsg = 0;
int value = 0;
char msg[100];
String DataString;
int checkUID();

WiFiClient espClient;
PubSubClient client(espClient);
MFRC522 rfid(SS_PIN, RST_PIN);

byte allowedUIDs[][4] = {
  //{0x62, 0x93, 0xA3, 0x51}, // First UID
  { 0x2F, 0x08, 0x00, 0x31 },  // JURE UID
  { 0x4F, 0x54, 0xCB, 0x66 },  // AungPao UID
  { 0xEF, 0x8A, 0x6F, 0xDD },  // Tae UID
  { 0xAD, 0x87, 0x49, 0xEF },
};
const int allowedUIDCount = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]);  // Number of allowed UIDs

const char* names[] = {
  "Thanatorn",
  "Natthawit",
  "Techawich",
  "Pao-Dormitory"
};

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {  //เชื่อมต่อกับ MQTT BROKER
      Serial.println("connected");
      client.subscribe("button/status");
      client.subscribe("project");  //Node-red
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 2 seconds");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    message = message + char(payload[i]);
  }
  Serial.println(message);
  if (String(topic) == "button/status") {
    if (message == "ON") {
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Door Open");
    } else if (message == "OFF") {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      Serial.println("Door Close");
    }
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  lcd.begin();
  lcd.backlight();
  WiFi.begin(ssid, password);  //เชื่อมต่อกับ WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());            //เชื่อมต่อกับ WIFI สำเร็จ แสดง IP
  client.setServer(mqtt_server, mqtt_port);  //กำหนด MQTT BROKER, PORT ที่ใช้
  client.setCallback(callback);              //ตั้งค่าฟังก์ชันที่จะทำงานเมื่อมีข้อมูลเข้ามาผ่านการ Subscribe
  client.subscribe("button/status");
  client.subscribe("project");
  pinMode(LED_PIN, OUTPUT);       // Set LED pin as output
  digitalWrite(LED_PIN, LOW);     // Start with the LED off
  pinMode(BUTTON_PIN, INPUT);     // Set button pin as input
  pinMode(BUZZER_PIN, OUTPUT);    // Set buzzer pin as output
  digitalWrite(BUZZER_PIN, LOW);  // Ensure buzzer is off initially
  pinMode(RELAY_PIN, OUTPUT);     // Set relay pin as output
  digitalWrite(RELAY_PIN, LOW);   // Ensure the relay is off initially
  pinMode(SENSOR_PIN, INPUT);
  Serial.println("Place your RFID card/tag near the reader...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 10000) {  //จับเวลาส่งข้อมูลทุก ๆ 5 วินาที
    lastMsg = now;
    ++value;


    // Example of data : {"data":{"temperature":25 , "humidity": 60}}
    DataString.toCharArray(msg, 100);
    client.publish("button/status", msg);  // อย่าลืมแก้ไข TOPIC ที่จะทำการ PUBLISH ไปยัง MQTT BROKER
  }
  delay(1);
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button Pressed. [Door OPEN]");
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay
    digitalWrite(LED_PIN, HIGH);    // Turn on the LED
    lcd.setCursor(0, 0);            // Set cursor to the first line
    lcd.print("Door Open");         // Print "Access Granted"
    exitStartTime = millis();       // Start the 10-second IR sensor check

    // Check IR sensor for 10 seconds //เปลี่ยนเป็น 5 วิ
    while (millis() - exitStartTime < 5000) {
      sensorValue = analogRead(SENSOR_PIN);
      if (sensorValue < threshold) {
        count--;  // Decrease count every time an object is detected
        Serial.print("Person Exited. Total Count: ");
        Serial.println(count);
        delay(1000);  // Wait 1 second before counting again to avoid rapid multiple detections
        totalCount = count;
      }
    }
    client.loop();
    long now = millis();
    lastMsg = now;
    ++value;
    DataString = (String)totalCount;
    DataString.toCharArray(msg, 100);
    client.publish("project", msg);
    if (!client.connected()) {
      reconnect();
    }
    delay(1000);                   // Relay active for 5 seconds
    digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
    digitalWrite(LED_PIN, LOW);
    lcd.clear();
  }
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("Card UID: ");
  String uidStr = "";  // Initialize a string to store UID for notification
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    uidStr += String(rfid.uid.uidByte[i], HEX);  // Append each byte to the uidStr
  }
  Serial.println();
  int uidIndex = checkUID();
  if (uidIndex != -1) {
    Serial.print("Authorized Card Detected ");
    Serial.println(names[uidIndex]);
    Serial.println("[Door OPEN]");
    newScanCount = 0;
    lcd.clear();                    // Clear the LCD
    lcd.setCursor(0, 0);            // Set cursor to the first line
    lcd.print("Welcome to KMITL");  // Print welcome message
    lcd.setCursor(0, 1);            // Set cursor to the second line
    lcd.print(names[uidIndex]);     // Display the corresponding name
    digitalWrite(LED_PIN, HIGH);    // Turn on the LED
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay (provide power)
    entryStartTime = millis();      // Start the 10-second IR sensor check
    String messageToPublish = names[uidIndex];
    String jsonMessage = String(names[uidIndex]);
    DataString = (String)totalCount;
    DataString.toCharArray(msg, 100);
    while (millis() - entryStartTime < 5000) {
      sensorValue = analogRead(SENSOR_PIN);
      if (sensorValue < threshold) {
        count++;  // Increase count every time an object is detected
        Serial.print("Person Entered. Total Count: ");
        Serial.println(count);
        delay(1000);  // Wait 1 second before counting again to avoid rapid multiple detections
      }
    }
    if (!client.connected()) {
      reconnect();
    }
    totalCount = count;
    client.loop();
    ++value;
    DataString = (String)totalCount;
    DataString.toCharArray(msg, 100);
    Serial.println(names[uidIndex]);
    client.publish("project", jsonMessage.c_str());
    client.publish("project", msg);  // อย่าลืมแก้ไข TOPIC ที่จะทำการ PUBLISH ไปยัง MQTT BROKER(name)

    delay(1000);                   // Relay active for 5 seconds
    digitalWrite(LED_PIN, LOW);    // Turn off the LED
    digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
    lcd.clear();
  } else {
    Serial.println("Unauthorized Card!");
    lcd.clear();                 // Clear the LCD
    lcd.setCursor(0, 0);         // Set cursor to the first line
    lcd.print("Access Denied");  // Print "Access Denied"
    client.publish("project", "Unknown Card");
    lcd.setCursor(0, 1);         // Set cursor to the second line
    lcd.print("Unknown Card");   // Display "Unknown Card"
    tone(BUZZER_PIN, 2000);      // Trigger buzzer for unauthorized card
    delay(3000);                 // Keep the buzzer on for 3 seconds
    lcd.clear();
    noTone(BUZZER_PIN);  // Turn off the buzzer
  }
  // Halt the RFID to stop the reader after each scan

  rfid.PICC_HaltA();
}
int checkUID() {
  if (rfid.uid.size != 4) {
    return -1;  // UID size must be 4 bytes
  }

  for (int i = 0; i < allowedUIDCount; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) {
      if (rfid.uid.uidByte[j] != allowedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;  // Return the index of the matched UID
    }
  }
  return -1;  // If no match found, return -1
}
