//-------------------- Radio Frequency Identification -------------------- //
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 7    // Define the pin for the LED
#define BUTTON_PIN 3 // Define the pin for the button
#define BUZZER_PIN 6 // Define the pin for the buzzer
#define RELAY_PIN 2  // Define the pin for the relay
#define SENSOR_PIN A0    // Define the pin for the E18-D80NK infrared sensor

#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6HhvIWJnA"
#define BLYNK_TEMPLATE_NAME "doorrable"
#define BLYNK_AUTH_TOKEN "2hxXYKqeycaPlyVO_yB6N5pu5lSEn4We"

#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>

// Your WiFi credentials.
char ssid[] = "aungpao";
char pass[] = "aungpaoaungpao";
/////////////////////////////////////////////////////
int count = 0;         // Variable to hold the count
int sensorValue = 0;     // Variable to hold the sensor reading
int threshold = 500;     // Define a threshold value (adjust based on testing)


MFRC522 rfid(SS_PIN, RST_PIN);

// List of allowed UIDs (each UID is 4 bytes long)
byte allowedUIDs[][4] = {
  //{0x62, 0x93, 0xA3, 0x51}, // First UID
  {0x2F, 0x08, 0x00, 0x31},  // JURE UID
  {0x4F, 0x54, 0xCB, 0x66} // AungPao UID
};
const int allowedUIDCount = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); // Number of allowed UIDs

// BLYNK_WRITE is called every time the button on the app is pressed or released
BLYNK_WRITE(V0)
{
  int val = param.asInt();  // Get the value from the button widget (0 or 1)
  
  if (val == 1) {
    tone(BUZZER_PIN, 2000);       // Send sound to buzzer at 1000Hz
    digitalWrite(RELAY_PIN, HIGH);  // Turn on the relay (set to HIGH)
    Serial.println("Relay ON");
  } else {
    noTone(BUZZER_PIN);           // Turn off the buzzer
    digitalWrite(RELAY_PIN, LOW);   // Turn off the relay (set to LOW)
    Serial.println("Relay OFF");
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  
  pinMode(LED_PIN, OUTPUT);   // Set LED pin as output
  digitalWrite(LED_PIN, LOW); // Start with the LED off

  pinMode(BUTTON_PIN, INPUT); // Set button pin as input

  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
  
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is off initially

  pinMode(SENSOR_PIN, INPUT); // Set the sensor pin as input
    // Initialize the relay pin as output

  // Initialize Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Alternatively, you can specify the Blynk server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  Serial.println("Place your RFID card/tag near the reader...");
}

void loop() {
  Blynk.run();
// Read the analog value from the sensor
  sensorValue = analogRead(SENSOR_PIN);
  Serial.println(sensorValue);
  delay(300);  // Small delay to stabilize the readings


  if (sensorValue < 500) {
    count++; // เพิ่มค่าตัวนับ
    Serial.print("Authorized Card Detected. Count: ");
    Serial.println(count);
    /*Serial.println("Authorized Card Detected. [Door OPEN]");
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay (provide power)
    delay(3000);                  // Keep the LED on for 2 seconds
    digitalWrite(LED_PIN, LOW);   // Turn off the LED
    digitalWrite(RELAY_PIN, LOW);   // Turn off the relay (cut power) */
  } 



  //-----//
  // Button control to turn on the LED for 5 seconds
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button Pressed. [Door OPEN]");
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay (provide power)
    delay(3000);                  // Keep the LED on for 5 seconds
    digitalWrite(LED_PIN, LOW);   // Turn off the LED
    digitalWrite(RELAY_PIN, LOW);   // Turn off the relay (cut power)

    Blynk.virtualWrite(V1, count);
  }

  // Check if a new card is present (RFID control is independent)
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Verify if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print card UID
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Check if the UID matches any allowed card
  if (checkUID()) {
    Serial.println("Authorized Card Detected. [Door OPEN]");
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay (provide power)
    delay(3000);                  // Keep the LED on for 2 seconds
    digitalWrite(LED_PIN, LOW);   // Turn off the LED
    digitalWrite(RELAY_PIN, LOW);   // Turn off the relay (cut power)
  } else {
    Serial.println("Unauthorized Card!"); // Triggering buzzer.
    digitalWrite(LED_PIN, LOW);   // Ensure the LED is off for unauthorized card
    tone(BUZZER_PIN, 2000);       // Send sound to buzzer at 1000Hz
    delay(3000);                  // Keep the buzzer on for 1 second
    noTone(BUZZER_PIN);           // Turn off the buzzer
  }

  // Halt the RFID to stop the reader after each scan
  rfid.PICC_HaltA();
}

// Function to check if the scanned UID matches any allowed UID
bool checkUID() {
  if (rfid.uid.size != 4) {
    return false;  // UID size must be 4 bytes
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
      return true;  // If one UID matches, return true
    }
  }

  return false;  // If no match found, return false
}
