#include <Servo.h>
#include <LiquidCrystal.h>


// --- Hardware Mapping and Constants ---
const byte LCD_RS = 12, LCD_EN = 11, LCD_D4 = 10, LCD_D5 = 9, LCD_D6 = 8, LCD_D7 = 7;
const byte LEFT_BUTTON = 5, RIGHT_BUTTON = 4;
const byte LEFT_LED = 3, RIGHT_LED = 2;
const byte SERVO_PIN = 6;

// --- System Configuration ---
const byte MIN_ANGLE = 0, MAX_ANGLE = 180;
const byte DEFAULT_ANGLE = 90;
// System fixed speed setting
const unsigned long UPDATE_INTERVAL = 100;

// --- Global Object and Variables ---
Servo servo;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

int servoPos = DEFAULT_ANGLE;
int lastPrintedPos = -1; // Set to -1 to force LCD to update on the first loop
bool limitReached = false;
unsigned long lastUpdateTime = 0; // Tracks time for the non-blocking timer

void setup() {
  lcd.begin(16, 2);
  lcd.print("System is ready");

  servo.attach(SERVO_PIN);
  servo.write(servoPos);

  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
}

// function to handle the top status line efficiency
void updateStatus(bool isLimit) {
  // Only write to the LCD if the status has actually changed
  if (limitReached != isLimit) {
    limitReached = isLimit;
    lcd.setCursor(0, 0);
    if (limitReached) lcd.print("Limit reached! ");
    else lcd.print("System is ready ");
  }
}

// Function to handle the bottom azimuth line efficiency
void updateAzimuth() {
  // Only write to the LCD if the position has changed
  if (servoPos != lastPrintedPos) {
    lcd.setCursor(0, 1);
    lcd.print("Azimut: ");
    lcd.print(servoPos);
    lcd.print("     ");
    lastPrintedPos = servoPos;
  }
}

void loop() {
  // Get current uptime in ms
  unsigned long currentMillis = millis();

  // Non-blocking timer: executes the movement logic only every UPDATE_INTERVAL
  if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = currentMillis; // Reset timer

    // Read hardware inputs
    bool leftPressed = (digitalRead(LEFT_BUTTON) == LOW);
    bool rightPressed = (digitalRead(RIGHT_BUTTON) == LOW);
    bool hitEdge = false;

    // Left movement logic
    if (leftPressed && !rightPressed) {
      digitalWrite(LEFT_LED, HIGH);
      if (servoPos > MIN_ANGLE) servoPos--;
      else hitEdge = true; // Mark that we cannot move further
    } else {
      digitalWrite(LEFT_LED, LOW);
    }

    // Right movement logic (prevents action if both buttons are pressed)
    if (rightPressed && !leftPressed) {
      digitalWrite(RIGHT_LED, HIGH);
      if (servoPos < MAX_ANGLE) servoPos++;
      else hitEdge = true; // Mark that we cannot move further
    } else {
      digitalWrite(RIGHT_LED, LOW);
    }

    // Determine system status based on edge detection and user input
    if (hitEdge && (leftPressed || rightPressed)) updateStatus(true);
    else if (limitReached && !hitEdge) updateStatus(false);

    // Execute physical movement and update display
    servo.write(servoPos);
    updateAzimuth();
  }
}