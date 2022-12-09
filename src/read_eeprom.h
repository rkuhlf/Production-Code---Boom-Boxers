#include <EEPROM.h>
#include <Arduino.h>

#include "config/eeprom_config.h"

void print_load_cell(int address, const char* prefix = "") {
    float load_cell;
    EEPROM.get(address, load_cell);
    Serial.print(prefix);
    Serial.print("Load cell calibrated to: ");
    Serial.println(load_cell);
}

void setup() {
    Serial.begin(57600);
    delay(1000);
    print_load_cell(LOAD_CELL_1_CAL, "Load Cell 1: ");
    print_load_cell(LOAD_CELL_2_CAL, "Load Cell 2: ");
    print_load_cell(LOAD_CELL_3_CAL, "Load Cell 3: ");
}

void loop() {
    
}