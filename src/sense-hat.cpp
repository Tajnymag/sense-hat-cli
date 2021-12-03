#include "sense-hat.hpp"

SenseHat::SenseHat(const char *hat_device_path) : hts221(hat_device_path), lps25H(hat_device_path),
                                                  lsm9Ds1(hat_device_path),
                                                  led2472G(hat_device_path) {}

double SenseHat::getHumidity() {
    this->hts221.powerUp();
    this->hts221.calibrate();
    return this->hts221.getHumidity();
}

double SenseHat::getPressure() {
    this->lps25H.powerUp();
    return this->lps25H.getPressure();
}

double SenseHat::getTemperature() {
    auto from_humidity = this->getTemperatureFromHumidity();
    auto from_pressure = this->getTemperatureFromPressure();

    return (from_humidity + from_pressure) / 2;
}

double SenseHat::getTemperatureFromHumidity() {
    this->hts221.powerUp();
    this->hts221.calibrate();
    return this->hts221.getTemperature();
}

double SenseHat::getTemperatureFromPressure() {
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

Vector3D SenseHat::getAngularRate() {
    auto sensor_reading = this->lsm9Ds1.getAngularRate();

    auto x = std::get<0>(sensor_reading);
    auto y = std::get<1>(sensor_reading);
    auto z = std::get<2>(sensor_reading);

    return Vector3D{x, y, z};
}

Vector3D SenseHat::getLinearAcceleration() {
    auto sensor_reading = this->lsm9Ds1.getLinearAcceleration();

    auto x = std::get<0>(sensor_reading);
    auto y = std::get<1>(sensor_reading);
    auto z = std::get<2>(sensor_reading);

    return Vector3D{x, y, z};
}

Vector3D SenseHat::getMagneticField() {
    auto sensor_reading = this->lsm9Ds1.getMagneticField();

    auto x = std::get<0>(sensor_reading);
    auto y = std::get<1>(sensor_reading);
    auto z = std::get<2>(sensor_reading);

    return Vector3D{x, y, z};
}
