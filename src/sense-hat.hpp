#ifndef SENSE_HAT_CLI_SENSE_HAT_HPP
#define SENSE_HAT_CLI_SENSE_HAT_HPP

#include <HTS221-driver/HTS221.h>
#include <LPS25H-driver/LPS25H.h>
#include <LSM9DS1-driver/LSM9DS1.h>
#include <LED2472G-driver/LED2472G.h>
#include <vector>

#define LED_MATRIX_SIZE 8

struct RGB {
    int r;
    int g;
    int b;
};

class SenseHat {
private:
    HTS221 hts221;
    LPS25H lps25H;
    LSM9DS1 lsm9Ds1;
    LED2472G led2472G;
public:
    explicit SenseHat(const char* hat_device_path);

    double getHumidity(unsigned int average_humidity_samples = 4);

    double getPressure(unsigned int average_pressure_samples = 2);

    double getTemperature(unsigned int  average_temperature_samples = 2);
    double getTemperatureFromHumidity(unsigned int  average_temperature_samples = 2);
    double getTemperatureFromPressure(unsigned int average_temperature_samples = 2);

    void putPixels(const std::vector<RGB> &pixels);
};


#endif //SENSE_HAT_CLI_SENSE_HAT_HPP
