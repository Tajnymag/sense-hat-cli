cmake_minimum_required(VERSION 3.10)
project(sense-hat-cli)

set(CMAKE_CXX_STANDARD 14)

set(DRIVERS_DIR thirdparty)
set(HTS221_DRIVER_DIR ${DRIVERS_DIR}/HTS221-driver)
set(LED2472G_DRIVER_DIR ${DRIVERS_DIR}/LED2472G-driver)
set(LPS25H_DRIVER_DIR ${DRIVERS_DIR}/LPS25H-driver)
set(LSM9DS1_DRIVER_DIR ${DRIVERS_DIR}/LSM9DS1-driver)

include_directories(${DRIVERS_DIR})

add_library(HTS221-driver
        ${HTS221_DRIVER_DIR}/HTS221.cpp
        ${HTS221_DRIVER_DIR}/HTS221.h
        ${HTS221_DRIVER_DIR}/HTS221_Types.h
        ${HTS221_DRIVER_DIR}/HTS221_Registers.h)

add_library(LED2472G-driver
        ${LED2472G_DRIVER_DIR}/LED2472G.cpp
        ${LED2472G_DRIVER_DIR}/LED2472G.h)

add_library(LPS25H-driver
        ${LPS25H_DRIVER_DIR}/LPS25H.cpp
        ${LPS25H_DRIVER_DIR}/LPS25H.h
        ${LPS25H_DRIVER_DIR}/LPS25H_Types.h
        ${LPS25H_DRIVER_DIR}/LPS25H_Registers.h)

add_library(LSM9DS1-driver
        ${LSM9DS1_DRIVER_DIR}/LSM9DS1.cpp
        ${LSM9DS1_DRIVER_DIR}/LSM9DS1.h
        ${LSM9DS1_DRIVER_DIR}/LSM9DS1_Types.h
        ${LSM9DS1_DRIVER_DIR}/LSM9DS1_Registers.h)

add_executable(sense-hat-cli src/main.cpp src/sense-hat.cpp)

target_link_libraries(sense-hat-cli HTS221-driver LED2472G-driver LPS25H-driver LSM9DS1-driver i2c)
