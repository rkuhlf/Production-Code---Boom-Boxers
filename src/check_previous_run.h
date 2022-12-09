#include <Arduino.h>
#include <EEPROM.h>

const int calVal_eepromAddress = 0;
const int data_start_eepromAddress = 8;
const int data_length_eepromAddress = 4;

const int STORABLE_VALUES = 230;
float data[STORABLE_VALUES];
int data_points_written = 0;

void print_data() {
    for (int i = 0; i < data_points_written; i++) {
        Serial.println(data[i]);
    }
}

void setup() {
    Serial.begin(57600); delay(10);

    Serial.println();
    Serial.println("Starting...");

    float recordedCalibrationFactor;
    EEPROM.get(calVal_eepromAddress, recordedCalibrationFactor);
    Serial.print("Used calibration factor of ");
    Serial.println(recordedCalibrationFactor);

    
    EEPROM.get(data_length_eepromAddress, data_points_written);
    Serial.print("Number of Measurements: ");
    Serial.println(data_points_written);

    EEPROM.get(data_start_eepromAddress, data);
    Serial.print("Measurements: ");
    print_data();
}