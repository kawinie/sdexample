#include <Arduino.h>

#include "SdFat.h"

constexpr auto SD_CS_PIN = 10;

void printContent(File dir, int numTabs = 0) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            break;
        }

        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }

        constexpr size_t size = 16;
        char nameBuffer[size];
        entry.getName(nameBuffer, size);

        if (entry.isDirectory()) {
            Serial.print(nameBuffer);
            Serial.println("/");
            printContent(entry, numTabs + 1);
        } else {
            Serial.print("* ");
            Serial.print(nameBuffer);
            Serial.print("\t\t");
            Serial.print((long) entry.size(), DEC);  // files have sizes, dirs do not
            Serial.println();
        }

        entry.close();
    }
}

void setup() {
    Serial.begin(115200);

    while (!Serial) {}

    // These are required to use Hypnos
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW);
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);

    auto sdconfig = SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(12));
    SdFat card;
    if (!card.begin(sdconfig)) {
        Serial.println("SD card init failed.");
        return;
    }

    File root = card.open("/");
    printContent(root);
    root.close();

    Serial.println("Done.");
}

void loop() {}