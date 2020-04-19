#include "esp32_hardware.h"
#include "hardware_interface.h"

double Esp32Hardware::getSecondsSinceStart() {
  return 0;
}

bool Esp32Hardware::readSensors(SensorState* state) {
  //pressure
  state->p1 = analogRead(PRESSURE_1);

  //flow
  state->f1 = flow1.read();

  return false;
}

const SensorState * Esp32Hardware::getSensorState() {
  return &sensorState;
}

void Esp32Hardware::updateSensorState() {
  readSensors(&sensorState);
}

bool Esp32Hardware::setValves(const ValveState& state) {
  return false;
}

//Perform a simple linear scale for our dials
double normalizeDial(double min, double max, int value, double divisions, bool invert) {
  double normalizedInput = ((double)value - DIAL_MIN) / (DIAL_MAX - DIAL_MIN);
  if (normalizedInput < 0) normalizedInput = 0;
  if (normalizedInput > 1) normalizedInput = 1;
  if (invert) normalizedInput = 1-normalizedInput;
  double normalizedOutput = min + normalizedInput * (max - min);
  int n = normalizedOutput / divisions;
  normalizedOutput = n * divisions;
  return normalizedOutput;
}

//TODO deprecate this function in favor of getControlState?
bool Esp32Hardware::readControls(ControlState* state) {
  state->target_switch = ports.digitalRead(PORTS_SWITCH2) ? ControlState::TargetSwitchState::VOLUME : ControlState::TargetSwitchState::PRESSURE;
  state->tidal_volume = normalizeDial(CONTROL_TIDAL_VOLUME_MIN,CONTROL_TIDAL_VOLUME_MAX,mux.analogRead(MUX_DIAL_4),CONTROL_TIDAL_VOLUME_DIVISIONS,CONTROL_TIDAL_VOLUME_INVERT);
  state->inspiratory_pressure = normalizeDial(CONTROL_INSP_PRESSURE_MIN,CONTROL_INSP_PRESSURE_MAX,mux.analogRead(MUX_DIAL_3),CONTROL_INSP_PRESSURE_DIVISIONS,CONTROL_INSP_PRESSURE_INVERT);
  state->rate_assist_switch = !ports.digitalRead(PORTS_SWITCH1);
  state->respiratory_rate = normalizeDial(CONTROL_RESP_RATE_MIN,CONTROL_RESP_RATE_MAX,mux.analogRead(MUX_DIAL_2),CONTROL_RESP_RATE_DIVISIONS,CONTROL_RESP_RATE_INVERT);
  state->inhale_exhale_ratio = normalizeDial(CONTROL_INHALE_RATIO_MIN,CONTROL_INHALE_RATIO_MAX,mux.analogRead(MUX_DIAL_1),CONTROL_INHALE_RATIO_DIVISIONS,CONTROL_INHALE_RATIO_INVERT);

  state->end_inspiratory_pause_button_down = pause.isPressed();
  state->start_ack_button_down = start.isPressed();
  return true;
}

ControlState * Esp32Hardware::getControlState() {
  return &controlState;
}

void Esp32Hardware::updateControlState() {
  ControlState currentState;
  readControls(&currentState);

  if (controlState.target_switch != currentState.target_switch ||
    controlState.tidal_volume != currentState.tidal_volume ||
    controlState.inspiratory_pressure != currentState.inspiratory_pressure ||
    controlState.rate_assist_switch != currentState.rate_assist_switch ||
    controlState.respiratory_rate != currentState.respiratory_rate ||
    controlState.inhale_exhale_ratio != currentState.inhale_exhale_ratio ||
    controlState.end_inspiratory_pause_button_down != currentState.end_inspiratory_pause_button_down ||
    controlState.start_ack_button_down != currentState.start_ack_button_down) controlStateUpdated = true;

  if (controlStateUpdated) {
    controlState.target_switch = currentState.target_switch;
    controlState.tidal_volume = currentState.tidal_volume;
    controlState.inspiratory_pressure = currentState.inspiratory_pressure;
    controlState.rate_assist_switch = currentState.rate_assist_switch;
    controlState.respiratory_rate = currentState.respiratory_rate;
    controlState.inhale_exhale_ratio = currentState.inhale_exhale_ratio;
    controlState.end_inspiratory_pause_button_down = currentState.end_inspiratory_pause_button_down;
    controlState.start_ack_button_down = currentState.start_ack_button_down;
  }
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

void Esp32Hardware::runTest() {
  screen.runTest();
}

//This contains testing/demo behavior that is not critical to operation
void Esp32Hardware::testModeTick() {
  if (start.hasPressed()) buzzer.shortBeep();
  if (pause.hasPressed()) buzzer.longBeep();

  IndicationState indicate;
  indicate.status_led_mode = controlState.target_switch == ControlState::PRESSURE ? IndicationState::SOLID_RED : IndicationState::SOLID_GREEN;
  writeIndication(indicate);
}

void Esp32Hardware::tick() {
  buzzer.tick();
  start.tick(!ports.digitalRead(PORTS_BUTTON1));
  pause.tick(!ports.digitalRead(PORTS_BUTTON2));
  updateControlState();

  testModeTick();

  if (controlStateUpdated) {
    controlStateUpdated = false;
    screen.tft.setCursor(0, 0);
    screen.tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    screen.tft.setTextSize(2);

    screen.tft.print("mode: ");
    screen.tft.print(controlState.target_switch == ControlState::PRESSURE ? "Pressure" : "Volume");
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("tidal_v: ");
    screen.tft.print(controlState.tidal_volume);
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("inspir_pressure: ");
    screen.tft.print(controlState.inspiratory_pressure);
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.println();

    screen.tft.print("rate assist: ");
    screen.tft.print(controlState.rate_assist_switch ? "ON" : "OFF");
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("resp_rate: ");
    screen.tft.print(controlState.respiratory_rate);
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("brth_ratio: ");
    screen.tft.print(controlState.inhale_exhale_ratio);
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("pause_btn: ");
    screen.tft.print(pause.isPressed());
    screen.tft.print("    ");
    screen.tft.println();

    screen.tft.print("start_btn: ");
    screen.tft.print(start.isPressed());
    screen.tft.print("    ");
    screen.tft.println();
  }
}

Esp32Hardware::Esp32Hardware() : HardwareInterface(),
  ports(I2C_1_SDA,I2C_1_SCL,IO_RST),
  mux(MUX0,MUX1,MUX2,MUXIO),
  screen(LCD_CS, LCD_DC, LCD_MOSI, LCD_SCLK, LCD_RST, LCD_MISO),
  buzzer(BUZZER, 0),
  flow1(I2C_1_SDA,I2C_1_SCL) {
}

void Esp32Hardware::begin() {
  ports.begin();
  mux.begin();
  screen.begin();
  buzzer.begin();
  flow1.begin();

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
