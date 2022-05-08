// Purpose: Smart Soap Dispenser Code
// Reference: https://bit.ly/2R0GNmt
// Author: A. Goldman
// Date: May 29, 2021
// Status: Working

#include <Wire.h>         // Includes the Wire library
#include <Adafruit_GFX.h> // Includes the libraries for the OLED display
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define RELAYPIN PB0     // Defines relay signal pin
#define IRPIN PC0        // Defines IR sensor pin
#define TOTALUSES 75     // Total times dispenser can be used
                         
// Configures display...
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
uint16_t uses = 0;           // Variable to count uses
uint16_t usesLeft;           // Variable to hold amount of uses left
uint16_t daysLeft;           // Variable to hold amount of days left
unsigned long millisCapture; // Time capture variable

void setup()
{
    DDRB |= 1 << RELAYPIN;                     // Sets relay pin as output
    DDRC &= ~(1 << IRPIN);                     // Sets IR pin as input
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Sets up display
    setupDisplay();                            // Runs custom function, created below
    display.setTextSize(1);                    // Sets text size
    display.setTextColor(WHITE);               // Sets text colour
    usesLeft = TOTALUSES - uses;               // Calculates number of uses left
    daysLeft = (usesLeft) / 7;                 // Calculated number of days left
}

void loop()
{
    setupDisplay();                        // Clears display and sets cursor to origin
    display.println("Place Hand Under");   // Writes text to display
    display.print("Sensor");               // Writes text to display
    display.setCursor(0, 30);              // Sets cursor to (0, 30)
    display.println("Canister Should be"); // Writes text to display...
    display.println(String("Replaced in ") + daysLeft + String(" Days"));
    display.println(String("or ") + usesLeft + String(" uses"));
    display.display(); // Displays text on display
    while (PINC & (1 << IRPIN)); // Wait until hand placed under sensor
    if (!(PINC & (1 << IRPIN)))
    {                              // If hand placed under sensor
        PORTB |= 1 << RELAYPIN;    // Activates relay by putting signal pin high
        delay(800);                // Delay while soap is being dispensed
        PORTB &= ~(1 << RELAYPIN); // Closes relay by putting signal pin low
        millisCapture = millis();  // Captures millis() time stamp
        while (millis() - millisCapture < 20000)
        { // Runs for 2 seconds...
            // Calculates number of seconds left for hand washing...
            uint8_t washSecondsLeft = 20 - ((millis() - millisCapture) / 1000);
            setupDisplay();                    // Clears display and sets cursor to origin
            display.println("Wash Hands for"); // Writes text to display...
            display.print(washSecondsLeft + String(" More Seconds"));
            display.display(); // Displays text on display
        }
        uses++;                      // Increases uses variable by 1
        usesLeft = TOTALUSES - uses; // Calculates uses left
        daysLeft = (usesLeft) / 7;   // Calculates number of days left
    }
}

void setupDisplay()
{                            // Function to initialize OLED display
    display.clearDisplay();  // Clears display
    display.setCursor(0, 0); // Sets cursor to origin
}
