#include "sense-hat.hpp"

SenseHat::SenseHat(const char *hat_device_path): hts221(hat_device_path), lps25H(hat_device_path), lsm9Ds1(hat_device_path),
                                                 led2472G(hat_device_path) {}

double SenseHat::getHumidity(const unsigned int average_humidity_samples) {
    this->hts221.powerUp();
    this->hts221.calibrate();
    return this->hts221.getHumidity();
}

double SenseHat::getPressure(const unsigned int average_pressure_samples) {
    this->lps25H.powerUp();
    this->lps25H.getPressure();
}

double SenseHat::getTemperature(const unsigned int average_temperature_samples) {
    auto from_humidity = this->getTemperatureFromHumidity(average_temperature_samples);
    auto from_pressure = this->getTemperatureFromPressure(average_temperature_samples);

    return (from_humidity + from_pressure) / 2;
}

double SenseHat::getTemperatureFromHumidity(const unsigned int average_temperature_samples) {
    this->hts221.powerUp();
    this->hts221.calibrate();
    return this->hts221.getTemperature();
}

double SenseHat::getTemperatureFromPressure(const unsigned int average_temperature_samples) {
    this->lps25H.powerUp();
    return this->lps25H.getTemperature();
}

void SenseHat::putPixels(const std::vector<RGB> &pixels) {
    for (unsigned int y = 0; y < LED_MATRIX_SIZE; ++y) {
        for (unsigned int x = 0; x < LED_MATRIX_SIZE; ++x) {
            auto pixel_position = (y * LED_MATRIX_SIZE) + x;
            if (pixel_position >= pixels.size()) break;

            auto pixel = pixels[pixel_position];

            this->led2472G.setPixelRGB(x, y, pixel.r, pixel.g, pixel.b);
        }
    }
    this->led2472G.update();
}
