#include "flow_sensor.h"
#include <Arduino.h>
#include <Wire.h>

#define FLOW_ADDRESS 0x40

// See:
// https://www.mouser.com/datasheet/2/682/Sensirion_Mass_Flow_Meters_SFM3200-AW_Datasheet-1524538.pdf
#define SFM3200_AW_OFFSET_FLOW 32768.0
#define SFM3200_AW_SCALE_FACTOR_FLOW 120.0


void FlowSensor::begin() {
  startContinuousMeasurement();
}

void FlowSensor::startContinuousMeasurement() {
	Wire.beginTransmission(byte(FLOW_ADDRESS));
	Wire.write(byte(0x10));
	Wire.write(byte(0x00));
	Wire.endTransmission();
}

float FlowSensor::read() {
  Wire.requestFrom(FLOW_ADDRESS, 3); // read 3 bytes from device with address 0x40
	uint16_t a = Wire.read(); // first received byte stored here. The variable "uint16_t" can hold 2 bytes, this will be relevant later
	uint8_t b = Wire.read(); // second received byte stored here
	uint8_t crc = Wire.read(); // crc value stored here
	uint8_t mycrc = 0xFF; // initialize crc variable
	mycrc = crc8(a, mycrc); // let first byte through CRC calculation
	mycrc = crc8(b, mycrc); // and the second byte too
	if (mycrc != crc) { // check if the calculated and the received CRC byte matches
		// TODO: figure out why CRC is incorrect and how to properly calculate that.


    // return -1;
		// Serial.println("Error: wrong CRC");
	}

	a = (a << 8) | b;

	float Flow = ((float)a - SFM3200_AW_OFFSET_FLOW) / SFM3200_AW_SCALE_FACTOR_FLOW;

	return Flow;
}

uint8_t FlowSensor::crc8(const uint8_t data, uint8_t crc) {
	   crc ^= data;
	   for ( uint8_t i = 8; i; --i ) {
		   crc = ( crc & 0x80 )
		   ? (crc << 1) ^ 0x31
		   : (crc << 1);
		}
	return crc;
}

FlowSensor::FlowSensor() {
}

FlowSensor::~FlowSensor() {
}
