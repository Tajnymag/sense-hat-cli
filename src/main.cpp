#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <getopt.h>

#include "sense-hat.hpp"

void print_sensor_data(SenseHat &sense_hat) {
    auto humidity = sense_hat.getHumidity();
    auto temperature_from_humidity = sense_hat.getTemperatureFromHumidity();

    auto pressure = sense_hat.getPressure();
    auto temperature_from_pressure = sense_hat.getTemperatureFromPressure();

    printf("humidity\t%.2f %% RH\n", humidity);
    printf("pressure\t%.2f hPa\n", pressure);
    printf("temperature_from_humidity\t%.2f °C\n", temperature_from_humidity);
    printf("temperature_from_pressure\t%.2f °C\n", temperature_from_pressure);
}

void update_display(SenseHat &sense_hat, std::istringstream &pixel_stream) {
    std::vector<RGB> pixel_array;

    while (not pixel_stream.eof()) {
        RGB pixel;

        pixel_stream >> pixel.r;
        pixel_stream >> pixel.g;
        pixel_stream >> pixel.b;

        pixel_array.push_back(pixel);
    }

    if (pixel_array.size() != LED_MATRIX_SIZE * LED_MATRIX_SIZE) {
        fprintf(stderr, "Led array should have %d values. The program received only %zu.", LED_MATRIX_SIZE * LED_MATRIX_SIZE, pixel_array.size());
        exit(1);
    }

    sense_hat.putPixels(pixel_array);
}

int main(int argc, char** argv) {
    std::string sense_hat_path = "/dev/i2c-2";
    std::istringstream pixel_stream{};

    int no_sensors = 0;

    const option long_opts[] = {
            {"no-sensors", no_argument, &no_sensors, 'n'},
            {"leds", required_argument, nullptr, 'l'},
            {nullptr, 0, nullptr, 0}
    };
    const auto short_opts = "nl:";

    int opt;
    while((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1){
        switch(opt){
            case 'l':
                pixel_stream = std::istringstream(optarg);
                break;
            case 'n':
                break;
            case ':':
                fprintf(stderr, "option needs a value\n");
                exit(1);
            case '?':
            default:
                fprintf(stderr, "unknown option: %c\n", optopt);
                exit(1);
        }
    }

    int should_print_sensors = !no_sensors;
    int should_update_display = not pixel_stream.eof();

    should_print_sensors = !no_sensors;

    SenseHat sense_hat(sense_hat_path.c_str());

    if (should_print_sensors) {
        print_sensor_data(sense_hat);
    }

    if (should_update_display) {
        update_display(sense_hat, pixel_stream);
        printf("leds_updated\tOK\n");
    }

    return 0;
}
