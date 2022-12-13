#include <HX711_ADC.h>
#include <string>

// #include <helpers/logger.h>
#include <EEPROM.h>

class LoadCellHookup
{
private:
    
    
public:
    LoadCellHookup(byte HX711_dout, byte HX711_sck, byte calVal_eepromAddress);
    ~LoadCellHookup();
    HX711_ADC LoadCell;
    float voltage;
    byte HX711_dout;
    byte HX711_sck;
    byte calVal_eepromAddress;
    float calibration_value;

    float get_mass()
    {
        if (LoadCell.update()) {
            voltage = LoadCell.getData();
        }

        return voltage * calibration_value;
    }

    float get_voltage()
    {
        if (LoadCell.update()) {
            voltage = LoadCell.getData();
        }

        return voltage;
    }

    bool setup() {
        LoadCell.begin();
        EEPROM.get(calVal_eepromAddress, calibration_value);        
        LoadCell.tare();

        return true;
    }
};


// Member initializer list.
LoadCellHookup::LoadCellHookup(byte HX711_dout_, byte HX711_sck_, byte calVal_eepromAddress_): LoadCell(HX711_dout_, HX711_sck_)
{
    HX711_dout = HX711_dout_;
    HX711_sck = HX711_sck_;
    calVal_eepromAddress = calVal_eepromAddress_;
}


LoadCellHookup::~LoadCellHookup()
{
    
}
