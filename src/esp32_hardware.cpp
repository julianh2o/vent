#include "esp32_hardware.h"
#include "hardware_interface.h"

double Esp32Hardware::getSecondsSinceStart() {
  return 0;
}

bool Esp32Hardware::readSensors(SensorState* state) {
  return false;
}

bool Esp32Hardware::setValves(const ValveState& state) {
  return false;
}

bool Esp32Hardware::readControls(ControlState* state) {
  state->target_switch = ports.digitalRead(PORTS_SWITCH2) ? ControlState::TargetSwitchState::VOLUME : ControlState::TargetSwitchState::PRESSURE;
  state->tidal_volume = mux.analogRead(MUX_DIAL_4); //TODO convert me
  state->inspiratory_pressure = mux.analogRead(MUX_DIAL_3); //TODO convert me
  state->rate_assist_switch = ports.digitalRead(PORTS_SWITCH1);
  state->respiratory_rate = mux.analogRead(MUX_DIAL_2); //TODO convert me
  state->inhale_exhale_ratio = mux.analogRead(MUX_DIAL_1); //TODO convert me
  state->end_inspiratory_pause_button_down = ports.digitalRead(PORTS_BUTTON2);
  state->start_ack_button_down = ports.digitalRead(PORTS_BUTTON1);
  return true;
}

bool Esp32Hardware::writeIndication(const IndicationState& state) {
  //TODO handle blinking states
  ports.digitalWrite(PORTS_LED1, state.status_led_mode == IndicationState::SOLID_RED);
  ports.digitalWrite(PORTS_LED2, state.status_led_mode == IndicationState::SOLID_GREEN);

  //TODO handle buzzer
  return true;
}

bool Esp32Hardware::updateDisplay(const DisplayState& state) {
  return false;
}

bool Esp32Hardware::getConfig(ConfigState* state) {
  return false;
}

Esp32Hardware::Esp32Hardware() : HardwareInterface(), ports(I2C_1_SDA,I2C_1_SCL), mux(MUX0,MUX1,MUX2,MUXIO) {
  ports.begin();
  mux.begin();

  ports.pinMode(PORTS_24V_1,OUTPUT);
  ports.pinMode(PORTS_24V_2,OUTPUT);
  ports.pinMode(PORTS_24V_3,OUTPUT);
  ports.pinMode(PORTS_LED1,OUTPUT);
  ports.pinMode(PORTS_LED2,OUTPUT);

  ports.pinMode(PORTS_F1_PWR,OUTPUT);
  ports.pinMode(PORTS_F1_HTR,OUTPUT);
  ports.pinMode(PORTS_BUTTON1,INPUT);
  ports.pinMode(PORTS_BUTTON2,INPUT);
  ports.pinMode(PORTS_SWITCH1,INPUT);
  ports.pinMode(PORTS_SWITCH2,INPUT);
  ports.pinMode(PORTS_F2_PWR,OUTPUT);
  ports.pinMode(PORTS_F2_HTR ,OUTPUT);

  mux.pinMode(INPUT);
}

Esp32Hardware::~Esp32Hardware() {
}
