// File should be uploaded to an Arduino that already has a calibration value saved at the Eeprom address.
// Baud has no effect; neither do random delays in the looping.

// Store the last five seconds of mass flow rates.
// When the mass flow rate has been negative more than 90% of the time in the last five seconds, start recording.

// Record the mass flow rate until it is decreasing for less than 60% of the five seconds.
// With the theory that it will be decreasing half the time when there is no flow and 100% of the time when it is flowing.


#include <Arduino.h>

#include <HX711_ADC.h>
#include <EEPROM.h>

#include "config/drain_hx711_config.h"
#include "config/eeprom_config.h"
#include "config/load_cell_wiring_config.h"

// Does not use the wrapper because it needs to store data differently.
// TODO: encapsulate this somehow so I do not have so much repetition.
// Go back to using EEPROM.
HX711_ADC LoadCell1(LOAD_CELL_1_DOUT, LOAD_CELL_1_SCK);
HX711_ADC LoadCell2(LOAD_CELL_2_DOUT, LOAD_CELL_2_SCK);
HX711_ADC LoadCell3(LOAD_CELL_3_DOUT, LOAD_CELL_3_SCK);

unsigned long time = 0;
const int STORABLE_VALUES = 64;
float data1[STORABLE_VALUES];
float data2[STORABLE_VALUES];
float data3[STORABLE_VALUES];
int last_written_index = 0;

float recordedCalibrationFactor1;
float recordedCalibrationFactor2;
float recordedCalibrationFactor3;

float cal1 = 0.06654563711;
float cal2 = 0.07054315246;
float cal3 = 0.06976268166;

void setup() {
    Serial.begin(57600);
    
    delay(1000);

    Serial.println();
    Serial.println("Starting...");

    Serial.println("Initializing load cells.");

    LoadCell1.begin();
    LoadCell2.begin();
    LoadCell3.begin();

    
    // EEPROM.get(LOAD_CELL_1_CAL, recordedCalibrationFactor1);
    recordedCalibrationFactor1 = 1.0f;
    Serial.print("(1): Using calibration factor of ");
    Serial.println(recordedCalibrationFactor1);
    LoadCell1.setCalFactor(recordedCalibrationFactor1);

    // EEPROM.get(LOAD_CELL_2_CAL, recordedCalibrationFactor2);
    recordedCalibrationFactor2 = 1.0f;
    Serial.print("(2): Using calibration factor of ");
    Serial.println(recordedCalibrationFactor2);
    LoadCell2.setCalFactor(recordedCalibrationFactor2);

    // EEPROM.get(LOAD_CELL_3_CAL, recordedCalibrationFactor3);
    recordedCalibrationFactor3 = 1.0f;
    Serial.print("(3): Using calibration factor of ");
    Serial.println(recordedCalibrationFactor3);
    LoadCell3.setCalFactor(recordedCalibrationFactor3);
    
    LoadCell1.tareNoDelay();
    LoadCell2.tareNoDelay();
    LoadCell3.tareNoDelay();

    Serial.println("Send 2 to begin recording values.");
    Serial.println("Send 4 to continually print output.");
}

bool recording_value = false;
bool printing_values = false;
unsigned long print_interval = 500;
unsigned long last_printed = 0;

float new_measurement1;
float new_measurement2;
float new_measurement3;

void handleNewData() {
    float total = new_measurement1 * cal1 + new_measurement2 * cal2 + new_measurement3 * cal3;

    if (printing_values && millis() > print_interval + last_printed) {
        Serial.print("Time: ");
        Serial.print(time);
        Serial.print(";     Load Cell output: ");
        Serial.println(total);
        
        Serial.print("Load Cell 1: ");
        Serial.print(new_measurement1 / recordedCalibrationFactor1);
        Serial.print("; Load Cell 2: ");
        Serial.print(new_measurement2 / recordedCalibrationFactor2);
        Serial.print("; Load Cell 3: ");
        Serial.println(new_measurement3 / recordedCalibrationFactor3);

        last_printed = millis();
    }

    if (recording_value) {
        data1[last_written_index] = new_measurement1;
        data2[last_written_index] = new_measurement2;
        data3[last_written_index] = new_measurement3;
        last_written_index++;

        if (last_written_index == STORABLE_VALUES) {
            Serial.println("Filled data slots");
            Serial.println("Send 1 to print the average.");
            recording_value = false;
        }
    }
}


float get_mean(float* arr) {
    float total = 0;
    for (int i = 0; i < STORABLE_VALUES; i++) {
        total += arr[i];
    }

    return total / STORABLE_VALUES;
}

void loop() {
    time = millis();

    // check for new data/start next conversion:
    if (LoadCell1.update()) {
        // For some reason getData outputs an int. Bruh.
        new_measurement1 = LoadCell1.getData();
        handleNewData();
    };

    if (LoadCell2.update()) {
        new_measurement2 = LoadCell2.getData();
        handleNewData();
    };

    if (LoadCell3.update()) {
        new_measurement3 = LoadCell3.getData();
        handleNewData();
    };


    if (Serial.available()) {
        int command = Serial.parseInt();
        Serial.println("Received command: ");
        Serial.println(command);

        if (command == 4) {
            Serial.println("Printing Values");
            printing_values = true;
        } else if (command == 3) {
            Serial.println("Not printing values");
            printing_values = false;
        } else if (command == 2) {
            Serial.println("Recording Values");
            recording_value = true;
            last_written_index = 0;
        } else if (command == 1) {
            recording_value = false;

            Serial.println("Average values are: ");
            Serial.println(get_mean(data1) / recordedCalibrationFactor1);
            Serial.println(get_mean(data2) / recordedCalibrationFactor2);
            Serial.println(get_mean(data3) / recordedCalibrationFactor3);
            Serial.print("Total: ");
            
            Serial.println(cal1 * get_mean(data1) + cal2 * get_mean(data2) + cal3 * get_mean(data3));

            Serial.println("Send 2 to begin recording values.");
        }
    }

}