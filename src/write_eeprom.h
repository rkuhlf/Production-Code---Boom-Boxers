#include <EEPROM.h>
#include <Arduino.h>

#include "config/eeprom_config.h"

void write_load_cell(int address, float value) {
    EEPROM.put(address, value);

    char message[256] = {0};
    strcat(message, "Wrote ");

    char str_value[32];
    dtostrf(value, 0, 5, str_value);
    strcat(message, str_value);

    strcat(message, " to EEPROM.");

    Serial.println(message);
}

void setup() {
    Serial.begin(57600);
    delay(1000);
    Serial.println("Writing EEPROM.");
    // Make sure to do it backwards of the sheets output.
    // 19.46679651	16.69976856	26.51780107
    write_load_cell(LOAD_CELL_1_CAL, 0.09050678);
    write_load_cell(LOAD_CELL_2_CAL, 0.05393877893);
    write_load_cell(LOAD_CELL_3_CAL, 0.07263319961);

    Serial.println("Complete");
}

void loop() {
    
}