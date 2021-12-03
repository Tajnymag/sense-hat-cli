#include <iostream>
#include <cstdio>
#include <vector>
#include <regex>

#include "sense-hat.hpp"

void print_sensor_data(SenseHat &sense_hat) {
    printf("humidity\t%.2f %%RH\n", sense_hat.getHumidity());
    printf("pressure\t%.2f hPa\n", sense_hat.getPressure());

    printf("temperature_from_humidity\t%.2f °C\n", sense_hat.getTemperatureFromHumidity());
    printf("temperature_from_pressure\t%.2f °C\n", sense_hat.getTemperatureFromPressure());

    auto angular_rate = sense_hat.getAngularRate();
    printf("angular_rate\t%.2f,%.2f,%.2f deg/s\n", angular_rate.x, angular_rate.y, angular_rate.z);

    auto linear_acceleration = sense_hat.getLinearAcceleration();
    printf("linear_acceleration\t%.2f,%.2f,%.2f G\n", linear_acceleration.x, linear_acceleration.y,
           linear_acceleration.z);

    auto magnetic_field = sense_hat.getMagneticField();
    printf("magnetic_field\t%.2f,%.2f,%.2f G\n", magnetic_field.x, magnetic_field.y, magnetic_field.z);
}

void update_display(SenseHat &sense_hat, std::istringstream &pixel_stream) {
    std::vector<RGB> pixel_array;

    while (true) {
        RGB pixel;

        if (pixel_stream.eof()) break;
        pixel_stream >> pixel.r;

        if (pixel_stream.eof()) break;
        pixel_stream >> pixel.g;

        if (pixel_stream.eof()) break;
        pixel_stream >> pixel.b;

        pixel_array.push_back(pixel);
    }

    if (pixel_array.size() != LED_MATRIX_SIZE * LED_MATRIX_SIZE) {
        fprintf(stderr, "Led array should have %d values. The program received only %zu.",
                LED_MATRIX_SIZE * LED_MATRIX_SIZE, pixel_array.size());
        exit(1);
    }

    sense_hat.putPixels(pixel_array);
}

int main(int argc, char **argv) {
    std::string sense_hat_path = "/dev/i2c-2";
    std::istringstream pixel_stream;
    bool should_print_sensors = true;
    bool should_update_display = false;

    std::string last_arg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--no-sensors") {
            should_print_sensors = false;
        } else if (arg == "--leds" || arg == "-d") {
            if (i == (argc - 1)) {
                fprintf(stderr,
                        "--leds arguments needs to be directly followed by a valid string array of RGB triplets!\n");
                exit(1);
            }
        } else if (last_arg == "--leds" || last_arg == "-d") {
            std::regex pixel_array_pattern(R"(^(?:\d+ \d+ \d+ ?){64}$)");
            if (not std::regex_match(arg, pixel_array_pattern)) {
                fprintf(stderr, "pixel array should be in a format (R G B){64} - \"255 255 255 128...\"\n");
                exit(1);
            }

            should_update_display = true;
            pixel_stream = std::istringstream(arg);
        } else if (arg == "--help" || arg == "-h") {
            printf("usage: sense-hat-cli [..options] SENSE_HAT_I2C_PATH\n");
            printf("options:\n");
            printf("--help|-h\t\tprints this help message\n");
            printf("--leds|-d RGB_ARRAY\t\tupdates the sense hat display with provided array of pixel values (fex. --leds \"255 255 255 128...\")\n");
            printf("--no-sensors\t\tdo not measure anything\n");
            exit(0);
        } else if (i == (argc - 1)) {
            sense_hat_path = arg;
        }

        last_arg = arg;
    }

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
