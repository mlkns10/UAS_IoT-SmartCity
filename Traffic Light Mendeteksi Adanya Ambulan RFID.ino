#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>

// RFID module setup
#define RST_PIN         5          // Pin 5 Arduino Mega Ke RFID
#define SS_PIN          53         // SS/SDA Pin 53 Arduino Mega Ke RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// I2C LCD address
#define I2C_ADDR 0x27

// Number of columns and rows on the LCD
#define LCD_COLS 16
#define LCD_ROWS 2

// RFID Card ID to detect (replace with your card's actual ID)
byte myCardID[] = {0x03, 0xE6, 0xDB, 0x1B};

// Initialize the LCD object

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

const int redPin1 = 22;
const int yellowPin1 = 23;
const int greenPin1 = 24;

const int redPin2 = 25;
const int yellowPin2 = 26;
const int greenPin2 = 27;

const int redPin3 = 28;
const int yellowPin3 = 29;
const int greenPin3 = 30;

// Waktu tunda untuk setiap fase (dalam milidetik)
const unsigned long greenTime = 7000;  // 7 detik
const unsigned long yellowTime = 2000; // 2 detik
const unsigned long redTime = 7000;    // 7 detik

unsigned long previousMillis = 0;

// Enum untuk definisi state
enum TrafficLightState {
  RED,
  YELLOW,
  GREEN
};

void setTrafficLight(int redPin, int yellowPin, int greenPin, TrafficLightState state) {
  digitalWrite(redPin, state == RED);
  digitalWrite(yellowPin, state == YELLOW);
  digitalWrite(greenPin, state == GREEN);
}

bool isAmbulanDetected = false;
  
void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  SPI.begin();
  mfrc522.PCD_Init();
  
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.backlight();
  lcd.clear();
  Serial.println("Initialization complete.");

  pinMode(redPin1, OUTPUT);
  pinMode(yellowPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);

  pinMode(redPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);

  pinMode(redPin3, OUTPUT);
  pinMode(yellowPin3, OUTPUT);
  pinMode(greenPin3, OUTPUT);

 
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Check if the detected card ID matches the specified ID
    if (compareCardID(mfrc522.uid.uidByte, myCardID)) {
      Serial.println("Ambulan card detected!");
      isAmbulanDetected = true;
      displayAmbulanMessage();
      
      // Traffic lights for Ambulan
      //Green
      digitalWrite(greenPin1, HIGH);
      digitalWrite(greenPin2, LOW);
      digitalWrite(greenPin3, LOW);
      //Kuning
      digitalWrite(yellowPin1, LOW);
      digitalWrite(yellowPin2, LOW);
      digitalWrite(yellowPin3, LOW);
      //Red
      digitalWrite(redPin2, HIGH);
      digitalWrite(redPin3, HIGH);
      digitalWrite(redPin1, LOW);


      delay(7000);  // Display "Ada Ambulan" for 5 seconds
      lcd.clear();
      
    }
    mfrc522.PICC_HaltA();
  } else {
    // If RFID card is not detected, reset the flag and set normal traffic lights
    isAmbulanDetected = false;
    Serial.println("Ambulan card not detected. Setting normal traffic lights."); 
  }

  ledaja();
  // Your existing LCD display code
  lcd.setCursor(0, 0);
  lcd.print("Lurus   : SLEMAN");
  lcd.setCursor(0, 1);
  lcd.print("Kelompok: 7 ");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Kiri    : TUGU");
  lcd.setCursor(0, 1);
  lcd.print("Kelompok: 7 ");
  delay(1000);
  lcd.clear();
 
  
  // Repeat for other LCD displays...
}

// Function to compare two RFID card IDs
bool compareCardID(byte* cardID1, byte* cardID2) {
  for (int i = 0; i < 4; i++) {
    if (cardID1[i] != cardID2[i]) {
      return false;
    }
  }
  return true;
}

// Function to display "Ada Ambulan" message on the LCD
void displayAmbulanMessage() {
  lcd.setCursor(6, 0);
  lcd.print("Ada");
  lcd.setCursor(4, 1);
  lcd.print("Ambulan");

}

void ledaja() {

  // Ambil waktu sekarang
  unsigned long currentMillis = millis();

  // Hitung durasi dari waktu terakhir
  unsigned long elapsedTime = currentMillis - previousMillis;

  // Tentukan state untuk setiap jalur lalu lintas
  TrafficLightState state1, state2, state3;

  if (elapsedTime < greenTime) {
    state1 = GREEN;
    state2 = RED;
    state3 = RED;
    } else if (elapsedTime < greenTime + yellowTime) {
    state1 = YELLOW;
    state2 = RED;
    state3 = RED;
    } else if (elapsedTime < 2 * greenTime + yellowTime) {
    state1 = RED;
    state2 = GREEN;
    state3 = RED;
    } else if (elapsedTime < 2 * greenTime + 2 * yellowTime) {
    state1 = RED;
    state2 = YELLOW;
    state3 = RED;
    } else if (elapsedTime < 3 * greenTime + 2 * yellowTime) {
    state1 = RED;
    state2 = RED;
    state3 = GREEN;
    } else if (elapsedTime < 3 * greenTime + 3 * yellowTime) {
    state1 = RED;
    state2 = RED;
    state3 = YELLOW;
  } else {
    ledaja();
  }
  setTrafficLight(redPin1, yellowPin1, greenPin1, state1);

  // Atur fase lampu lalu lintas untuk Jalur 2
  setTrafficLight(redPin2, yellowPin2, greenPin2, state2);

  // Atur fase lampu lalu lintas untuk Jalur 3
  setTrafficLight(redPin3, yellowPin3, greenPin3, state3);

}
