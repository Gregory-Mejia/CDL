#include <LiquidCrystal.h>

// LCD Pin Mapping (Direct Wiring)
const int rs = 13, en = 12, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String inputString = "";         // A string to hold incoming data
bool stringComplete = false;     // Whether the string is complete

void setup() {
   Serial.begin(115200);
    //esp32 at baud rate of 115200
  
  lcd.begin(16, 2);
  lcd.print("Reading ESP data");
  // Reserve space for the message
  inputString.reserve(200);
}

void loop() {
  // Checks for a line with distance info
  if (stringComplete) {
    // Look for "distance" in the ESPresense logs
    if (inputString.indexOf("distance") > 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Device Found!");

      // Shows a snippet of the log on the second row
      lcd.setCursor(0, 1);
      lcd.print(inputString.substring(0, 16));
    }

    // Clear the string for the next line
    inputString = "";
    stringComplete = false;
  }
}
// Runs automatically when serial data from the ESP is detected
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    // If the incoming character is a newline, the string is complete
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
