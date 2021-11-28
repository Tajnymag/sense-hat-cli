#include <iostream>
#include <cstdio>

#include <getopt.h>

#include <HTS221-driver/HTS221.h>
#include <LPS25H-driver/LPS25H.h>
#include <LSM9DS1-driver/LSM9DS1.h>
#include <LED2472G-driver/LED2472G.h>

const option long_opts[] = {
        {"leds", required_argument, nullptr, 'l' }
};
const auto short_opts = "l:";

void print_help() {
    printf("Usage: sense-hat-cli [options...] <hat_device_path>\n");
}

int main(int argc, char** argv) {
    std::string sense_hat_path = "/dev/i2c-2";
    std::string leds;

    int opt;
    while((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1){
        switch(opt){
            case 'l':
                leds = optarg;
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }

    }

    for (; optind < argc; optind++){
        sense_hat_path = argv[optind];
    }

    auto hts221 = HTS221(sense_hat_path.c_str());
    auto lps25h = LPS25H(sense_hat_path.c_str());
    auto lsm9ds1 = LSM9DS1(sense_hat_path.c_str());
    auto led2472g = LED2472G(sense_hat_path.c_str());

    hts221.powerUp();
    lps25h.powerUp();
    hts221.calibrate();

    auto humidity = hts221.getHumidity();
    auto temperature_from_humidity = hts221.getTemperature();

    auto pressure = lps25h.getPressure();
    auto temperature_from_pressure = lps25h.getTemperature();

    printf("humidity\t%.2f %% RH\n", humidity);
    printf("pressure\t%.2f hPa\n", pressure);
    printf("temperature_from_humidity\t%.2f °C\n", temperature_from_humidity);
    printf("temperature_from_pressure\t%.2f °C\n", temperature_from_pressure);

    return 0;
}
