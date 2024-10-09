//-------------------- Radio Frequency Identification -------------------- //
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 7    // Define the pin for the LED
#define BUTTON_PIN 3 // Define the pin for the button
#define BUZZER_PIN 6 // Define the pin for the buzzer
#define RELAY_PIN 2  // Define the pin for the relay
#define SENSOR_PIN A1    // Define the pin for the E18-D80NK infrared sensor

#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6HhvIWJnA"
#define BLYNK_TEMPLATE_NAME "doorrable"
#define BLYNK_AUTH_TOKEN "2hxXYKqeycaPlyVO_yB6N5pu5lSEn4We"

#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define a virtual pin for notifications
#define NOTIFY_VPIN V3
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Your WiFi credentials.
char ssid[] = "aungpao";
char pass[] = "aungpaoaungpao";
/////////////////////////////////////////////////////

int count = 0;         // Variable to hold the count
int sensorValue = 0;     // Variable to hold the sensor reading
int threshold = 500;     // Define a threshold value (adjust based on testing)
unsigned long entryStartTime = 0;
unsigned long exitStartTime = 0;



MFRC522 rfid(SS_PIN, RST_PIN);

// List of allowed UIDs (each UID is 4 bytes long)
byte allowedUIDs[][4] = {
  //{0x62, 0x93, 0xA3, 0x51}, // First UID
  {0x2F, 0x08, 0x00, 0x31},  // JURE UID
  {0x4F, 0x54, 0xCB, 0x66}, // AungPao UID
  {0xEF, 0x8A, 0x6F, 0xDD}, // Tae UID
  {0xAD, 0x87, 0x49, 0xEF},
};
const int allowedUIDCount = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); // Number of allowed UIDs

// Corresponding names for each UID
const char* names[] = {
  "Thanatorn",
  "Natthawit",
  "Techawich",
  "Pao-Dormitory"
};

// Function prototypes
int checkUID();

// BLYNK_WRITE is called every time the button on the app is pressed or released
BLYNK_WRITE(V0)
{
  int val = param.asInt();  // Get the value from the button widget (0 or 1)
  
  if (val == 1) {
    tone(BUZZER_PIN, 2000);       // Send sound to buzzer at 1000Hz
    digitalWrite(RELAY_PIN, HIGH);  // Turn on the relay (set to HIGH)
    Serial.println("Blynk : Relay ON");
  } else {
    noTone(BUZZER_PIN);           // Turn off the buzzer
    digitalWrite(RELAY_PIN, LOW);   // Turn off the relay (set to LOW)
    Serial.println("Blynk : Relay OFF");
  }
}

// Function prototypes


void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  lcd.begin();
  lcd.backlight();

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
  // Add this function to send Blynk notifications
  void sendBlynkNotification(const char* name, int count) {
    String message = String("RFID Scan: ") + name + " has entered. Total count: " + count;
    Blynk.virtualWrite(NOTIFY_VPIN, message); // Send message to the virtual pin
}

void loop() {
  Blynk.run();

  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button Pressed. [Door OPEN]");
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    lcd.setCursor(0, 0); // Set cursor to the first line
    lcd.print("Door Open"); // Print "Access Granted"
    exitStartTime = millis();       // Start the 10-second IR sensor check

    // Check IR sensor for 10 seconds //เปลี่ยนเป็น 5 วิ
    while (millis() - exitStartTime < 5000) {
      sensorValue = analogRead(SENSOR_PIN);
      if (sensorValue < threshold) {
        count--;  // Decrease count every time an object is detected
        Serial.print("Person Exited. Total Count: ");
        Serial.println(count);
        delay(1000);  // Wait 1 second before counting again to avoid rapid multiple detections
      }
    }
    delay(1000);  // Relay active for 5 seconds
    digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
    digitalWrite(LED_PIN, LOW); 
    lcd.clear(); 

    // Send the updated count to Blynk virtual pin V2
    Blynk.virtualWrite(V2, count);
  }


  // Check if a new card is present (RFID control is independent)
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Verify if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print card UID and display it on the LCD
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Print each byte of the UID to the serial monitor and LCD
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();




  // Check if the UID matches any allowed card
  int uidIndex = checkUID();
  if (uidIndex != -1) {
    Serial.println("Authorized Card Detected. [Door OPEN]");

    lcd.clear();  // Clear the LCD
    lcd.setCursor(0, 0); // Set cursor to the first line
    lcd.print("Welcome to KMITL"); // Print welcome message
    lcd.setCursor(0, 1); // Set cursor to the second line
    lcd.print(names[uidIndex]); // Display the corresponding name
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(RELAY_PIN, HIGH);  // Activate the relay (provide power)
    entryStartTime = millis();      // Start the 10-second IR sensor check

    // Check IR sensor for 10 seconds //เปลี่ยนเป็น 5 วิ
    while (millis() - entryStartTime < 5000) {
      sensorValue = analogRead(SENSOR_PIN);
      if (sensorValue < threshold) {
          count++;  // Increase count every time an object is detected
          Serial.print("Person Entered. Total Count: ");
          Serial.println(count);
          delay(1000);  // Wait 1 second before counting again to avoid rapid multiple detections
      }
    }

    sendBlynkNotification(names[uidIndex], count);

    delay(1000); // Relay active for 5 seconds
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
    lcd.clear();

    // Send the updated count to Blynk virtual pin V2
    Blynk.virtualWrite(V2, count);
  } else {
    Serial.println("Unauthorized Card!");
    lcd.clear();  // Clear the LCD
    lcd.setCursor(0, 0); // Set cursor to the first line
    lcd.print("Access Denied"); // Print "Access Denied"
    lcd.setCursor(0, 1); // Set cursor to the second line
    lcd.print("Unknown Card"); // Display "Unknown Card"
    tone(BUZZER_PIN, 2000);  // Trigger buzzer for unauthorized card
    delay(3000);  // Keep the buzzer on for 3 seconds
    lcd.clear(); 
    noTone(BUZZER_PIN);  // Turn off the buzzer
  }

  // Halt the RFID to stop the reader after each scan
  rfid.PICC_HaltA();
}


// Function to check if the scanned UID matches any allowed UID
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
