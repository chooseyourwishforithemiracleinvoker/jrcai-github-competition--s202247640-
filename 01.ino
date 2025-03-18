#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define TRANSISTOR_PIN A0 // Analog pin to read the transistor state

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Transistor State:");

  // Set RGB LED pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Set transistor pin as input
  pinMode(TRANSISTOR_PIN, INPUT);

  // Debugging
  Serial.begin(9600);
}

void loop() {
  // Function to read the transistor state continuously
  updateTransistorState();

  // 1️⃣ Blue → Cyan (Increase Green)
  for (int g = 0; g <= 255; g++) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, 255);
    delay(5);
    updateTransistorState(); // Read while transitioning
  }

  // 2️⃣ Cyan → Green (Remove Blue)
  for (int b = 255; b >= 0; b--) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, b);
    delay(5);
    updateTransistorState();
  }

  // 3️⃣ Green → Yellow (Increase Red)
  for (int r = 0; r <= 255; r++) {
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
    delay(5);
    updateTransistorState();
  }

  // 4️⃣ Yellow → Orange (Reduce Green)
  for (int g = 255; g >= 127; g--) { // Reduce Green to shift to Orange
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, 0);
    delay(5);
    updateTransistorState();
  }

  // 5️⃣ Orange → Red (Remove Green)
  for (int g = 127; g >= 0; g--) {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, 0);
    delay(5);
    updateTransistorState();
  }

  // 6️⃣ Show White (All LEDs ON)
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);
  delay(1000); // Show white for 1 second

  // Loop back to blue and repeat
}

// Function to update transistor state in real-time
void updateTransistorState() {
  int transistorState = analogRead(TRANSISTOR_PIN);

  // Clear the previous state on the LCD
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear line

  // Display the updated value
  lcd.setCursor(0, 1);
  lcd.print("Transistor: ");
  lcd.print(transistorState);

  // Debugging output
  Serial.print("Transistor: ");
  Serial.println(transistorState);
}