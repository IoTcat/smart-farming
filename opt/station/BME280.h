#ifndef  BME280_H_
#define  BME280_H_
#include <Wire.h>
#include <Arduino.h>


class BME280
{
#define BME280_ADDRESS 0x76

private:

    signed long int t_fine;
    uint16_t dig_T1;
     int16_t dig_T2;
     int16_t dig_T3;
    uint16_t dig_P1;
     int16_t dig_P2;
     int16_t dig_P3;
     int16_t dig_P4;
     int16_t dig_P5;
     int16_t dig_P6;
     int16_t dig_P7;
     int16_t dig_P8;
     int16_t dig_P9;
     int8_t  dig_H1;
     int16_t dig_H2;
     int8_t  dig_H3;
     int16_t dig_H4;
     int16_t dig_H5;
     int8_t  dig_H6;

public:
unsigned long int hum_raw,temp_raw,pres_raw;

void readTrim();

void writeReg(uint8_t reg_address, uint8_t data);

void readData();

double getTemp();

double getPress();

double getHum();

signed long int calibration_T(signed long int adc_T);

unsigned long int calibration_P(signed long int adc_P);

unsigned long int calibration_H(signed long int adc_H);
};
#endif
