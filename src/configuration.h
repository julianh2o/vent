#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Minimum and maximum raw readings on the dials
#define DIAL_MIN 0
#define DIAL_MAX 4049

//Control scale for each dial control
#define CONTROL_TIDAL_VOLUME_MIN 50
#define CONTROL_TIDAL_VOLUME_MAX 1500
#define CONTROL_TIDAL_VOLUME_DIVISIONS 50
#define CONTROL_TIDAL_VOLUME_INVERT true

#define CONTROL_INSP_PRESSURE_MIN .26
#define CONTROL_INSP_PRESSURE_MAX .57
#define CONTROL_INSP_PRESSURE_DIVISIONS .01
#define CONTROL_INSP_PRESSURE_INVERT true

#define CONTROL_RESP_RATE_MIN 5
#define CONTROL_RESP_RATE_MAX 30
#define CONTROL_RESP_RATE_DIVISIONS 1
#define CONTROL_RESP_RATE_INVERT true

#define CONTROL_INHALE_RATIO_MIN 1
#define CONTROL_INHALE_RATIO_MAX 4
#define CONTROL_INHALE_RATIO_DIVISIONS .25
#define CONTROL_INHALE_RATIO_INVERT true

//Ports
#define MUX0 26
#define MUX1 27
#define MUX2 14
#define MUXIO 33

//Port expander portA
#define PORTS_24V_1 0
#define PORTS_24V_2 1
#define PORTS_24V_3 2
#define PORTS_LED1 3
#define PORTS_LED2 4

//Port expander portB
#define PORTS_F1_PWR 8
#define PORTS_F1_HTR 9
#define PORTS_BUTTON1 10
#define PORTS_BUTTON2 11
#define PORTS_SWITCH1 12
#define PORTS_SWITCH2 13
#define PORTS_F2_PWR 14
#define PORTS_F2_HTR 15

//Mux ports
#define MUX_DAC 0
#define MUX_DIAL_1 1
#define MUX_DIAL_2 2
#define MUX_DIAL_3 3
#define MUX_DIAL_4 4
#define MUX_V33 5
#define MUX_TH1AN 6
#define MUX_TH2AN 7

#define LCD_CS 5
#define LCD_DC 15
#define LCD_MOSI 23
#define LCD_SCLK 18
#define LCD_RST 12
#define LCD_MISO 19

#define I2C_1_SDA 21
#define I2C_1_SCL 22

#define I2C_2_SDA 13
#define I2C_2_SCL 4

#define PRESSURE_1 36
#define PRESSURE_2 39
#define PRESSURE_3 34
#define PRESSURE_4 35

#define BUZZER 32

#define IO_RST 2

#endif  // CONFIGURATION_H
