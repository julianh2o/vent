#include "esp32_hardware.h"
#include "hardware_interface.h"

#include "screen.h"

const int width = 320;
const int boxHeight = 50;
const int divs = 4;
const int padding = 3;
const int boxWidth = ((width-padding) / divs) - padding;

void Esp32Hardware::restartUptime() {
  timeval tv;
  gettimeofday(&tv, NULL);
  bootTimestamp = tv.tv_sec;
}

double Esp32Hardware::getSecondsSinceStart() {
  //
  // IMPORTANT
  // =========
  //
  // This implementation was on ESP32 with Arduino IDE with
  // ARDUINOJSON_USE_DOUBLE set to 1. It also should work with FLOAT as it
  // debases number of seconds by the timestamp of when the instance of
  // TimeCounter created. Be aware that FLOAT precision is not enough for
  // storing number of seconds since Unix Epoch. Be careful when working with
  // timestamps on embedded platform.
  //
  timeval tv;
  gettimeofday(&tv, NULL);
  time_t seconds_since_creation_time_t = tv.tv_sec - bootTimestamp;
  double s = (double)seconds_since_creation_time_t;
  double ms = (double)(tv.tv_usec / 1000LL);
  return s + ms / 1000.0;
}

bool Esp32Hardware::readSensors(SensorState* state) {
  //pressure
  state->p1 = 4095.0 - analogRead(PRESSURE_1);

  // flow
  state->f1 = flow1.read();

  return false;
}

const SensorState * Esp32Hardware::getSensorState() {
  return &sensorState;
}

void Esp32Hardware::updateSensorState() {
  readSensors(&sensorState);
}

bool Esp32Hardware::setValves(bool v1, bool v2) {
  ports.digitalWrite(PORTS_24V_1,v1);
  ports.digitalWrite(PORTS_24V_2,v2);
  return true;
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
  start.tick(!ports.digitalRead(PORTS_BUTTON1));
  pause.tick(!ports.digitalRead(PORTS_BUTTON2));


  readControls(&controlState);
}

bool Esp32Hardware::writeIndication(const IndicationState& state) {

  //
  // TODO handle blinking states
  //
  ports.digitalWrite(PORTS_LED1, state.status_led_mode == IndicationState::SOLID_GREEN);
  ports.digitalWrite(PORTS_LED2, state.status_led_mode == IndicationState::SOLID_RED);

  buzzer.setState(state.beeper_mode);

  return true;
}

bool Esp32Hardware::updateDisplay(const DisplayState& state) {
  return false;
}

const ConfigState* Esp32Hardware::getConfig() {
  return &configState;
}


void Esp32Hardware::runTest() {
  Screen::getInstance()->runTest();
}

//This contains testing/demo behavior that is not critical to operation
void Esp32Hardware::testModeTick() {
  /*
  if (start.hasPressed()) buzzer.shortBeep();
  if (pause.hasPressed()) buzzer.longBeep();
*/
  IndicationState indicate;
  indicate.status_led_mode = controlState.target_switch == ControlState::PRESSURE ? IndicationState::SOLID_RED : IndicationState::SOLID_GREEN;
  writeIndication(indicate);
}

void Esp32Hardware::boxTextTop(uint8_t n) {
    Screen::getInstance()->tft.setCursor(3 + n*(boxWidth + padding),240-boxHeight + 3);
    Screen::getInstance()->tft.setTextSize(1);
}

void Esp32Hardware::boxTextBottom(uint8_t n) {
    Screen::getInstance()->tft.setCursor(3 + n*(boxWidth + padding),240-boxHeight + 21);
    Screen::getInstance()->tft.setTextSize(3);
}

void Esp32Hardware::tick() {

  Screen* screen = Screen::getInstance();
  buzzer.tick();
  updateControlState();
  updateSensorState();

  bool refresh = false;
  if (forceRefresh || controlState.target_switch != lastControlState.target_switch || controlState.rate_assist_switch != lastControlState.rate_assist_switch) {
    refresh = true;
    forceRefresh = false;
    screen->clear();
  }

  screen->tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  screen->tft.setTextSize(2);

  //normalizeDial(0,20,mux.analogRead(MUX_DIAL_3),1,true); can use this for testing gfx positioning..
  uint8_t lineHeight = 18;

  if (controlState.target_switch == ControlState::VOLUME) {
    if (refresh) {
      screen->tft.setCursor(0, 0);
      screen->tft.println("Mode: ACV Target Volume");
    }

    if (refresh || controlState.tidal_volume != lastControlState.tidal_volume) {
      screen->tft.setCursor(0, lineHeight);
      screen->tft.print("Target Volume: ");
      screen->padprint("%.0f cc",controlState.tidal_volume,7);
      screen->tft.println();
    }

    if (refresh || controlState.inspiratory_pressure != lastControlState.inspiratory_pressure) {
      screen->tft.setCursor(0, 2*lineHeight);
      screen->tft.print("Alert Pressure: ");
      screen->padprint("%.2f cm",controlState.inspiratory_pressure,7);
      screen->tft.println();
    }
  } else {
    if (refresh) {
      screen->tft.setCursor(0, 0);
      screen->tft.println("Mode: ACV Target Pressure");
    }

    if (refresh || controlState.inspiratory_pressure != lastControlState.inspiratory_pressure) {
      screen->tft.setCursor(0, lineHeight);
      screen->tft.print("Target Pressure: ");
      screen->padprint("%.2f cm",controlState.inspiratory_pressure,7);
      screen->tft.println();
    }

    if (refresh || controlState.tidal_volume != lastControlState.tidal_volume) {
      screen->tft.setCursor(0, 2*lineHeight);
      screen->tft.print("Alert Volume: ");
      screen->padprint("%.0f cc",controlState.tidal_volume,7);
      screen->tft.println();
    }
  }

  if (controlState.rate_assist_switch) {
    if (refresh || controlState.respiratory_rate != lastControlState.respiratory_rate) {
      screen->tft.setCursor(0, 3*lineHeight);
      screen->tft.print("Min resp rate: ");
      screen->padprint("%.0f bpm",controlState.respiratory_rate,6);
      screen->tft.println();
    }

    if (refresh || controlState.inhale_exhale_ratio != lastControlState.inhale_exhale_ratio) {
      screen->tft.setCursor(0, 4*lineHeight);
      screen->tft.print("Resp Ratio: 1:");
      screen->padprint("%.2f",controlState.inhale_exhale_ratio,9);
      screen->tft.println();
    }
  } else {
    if (refresh) {
      screen->tft.setCursor(0, 3*lineHeight);
      screen->tft.println("Rate assist OFF");
    }
  }

  if (refresh) {
    screen->tft.setCursor(0, 6*lineHeight);
    screen->tft.println("Measurements: ");
  }

  if (refresh || sensorState.p1 != lastSensorState.p1) {
    screen->tft.setCursor(0, 6*lineHeight);
    screen->tft.println("Measurements: ");
    screen->tft.print("Pressure: ");
    screen->tft.print(sensorState.p1);
    screen->tft.print("    ");
    screen->tft.println();
  }

  if (refresh || sensorState.f1 != lastSensorState.f1) {
    screen->tft.setCursor(0, 8*lineHeight);
    screen->tft.print("Flow: ");
    if (sensorState.f1 == -1) {
      screen->tft.print("ERR");
    } else {
      screen->tft.print(sensorState.f1);
    }
    screen->tft.print("    ");
    screen->tft.println();
  }

  //width=320, height=240
  if (refresh) {
    for (int i=0; i<divs; i++) {
      screen->tft.fillRect(i*(boxWidth+padding), 240-boxHeight, boxWidth, boxHeight-1, ILI9341_BLACK);
      screen->tft.drawRect(i*(boxWidth+padding), 240-boxHeight, boxWidth, boxHeight-1, ILI9341_WHITE);
    }
  }

  //Tidal volume
  if (refresh || controlState.tidal_volume != lastControlState.tidal_volume) {
    boxTextTop(0);
    screen->tft.print("TVol [cc]");
    boxTextBottom(0);
    screen->padprint("%.0f",controlState.tidal_volume,4);
  }

  //Insp pressure
  if (refresh || controlState.inspiratory_pressure != lastControlState.inspiratory_pressure) {
    boxTextTop(1);
    screen->tft.print("Insp [cm]");
    boxTextBottom(1);
    screen->padprint("%.2f",controlState.inspiratory_pressure,4);
  }

  //Resp rate
  if (refresh || controlState.respiratory_rate != lastControlState.respiratory_rate) {
    screen->tft.setTextColor(controlState.rate_assist_switch ? ILI9341_WHITE : ILI9341_DARKGREY, ILI9341_BLACK);
    boxTextTop(2);
    screen->tft.print("Rate [bpm]");

    boxTextBottom(2);
    screen->padprint("%.0f",controlState.respiratory_rate,2);
  }

  //Inhale/Exhale Ratio
  if (refresh || controlState.inhale_exhale_ratio != lastControlState.inhale_exhale_ratio) {
    screen->tft.setTextColor(controlState.rate_assist_switch ? ILI9341_WHITE : ILI9341_DARKGREY, ILI9341_BLACK);
    boxTextTop(3);
    screen->tft.print("in/ex ratio");

    boxTextBottom(3);
    screen->padprint("%.2f",controlState.inhale_exhale_ratio,4);
  }

  lastControlState.target_switch = controlState.target_switch;
  lastControlState.tidal_volume = controlState.tidal_volume;
  lastControlState.inspiratory_pressure = controlState.inspiratory_pressure;
  lastControlState.rate_assist_switch = controlState.rate_assist_switch;
  lastControlState.respiratory_rate = controlState.respiratory_rate;
  lastControlState.inhale_exhale_ratio = controlState.inhale_exhale_ratio;
  lastControlState.end_inspiratory_pause_button_down = controlState.end_inspiratory_pause_button_down;
  lastControlState.start_ack_button_down = controlState.start_ack_button_down;

  lastSensorState.p1 = sensorState.p1;
  lastSensorState.f1 = sensorState.f1;
}

Esp32Hardware::Esp32Hardware() : HardwareInterface(),
  ports(I2C_1_SDA,I2C_1_SCL,IO_RST),
  mux(MUX0,MUX1,MUX2,MUXIO),
  buzzer(BUZZER_PIN,0),
  flow1() {

  forceRefresh = true;

  // TODO: Populate config
  configState.p_in = 2700.;
  configState.p_ex = 3900.;
  configState.t_tex = 2.0; // TODO: Calculate this on control change
  configState.f_stop = 3.0;

}

void Esp32Hardware::begin() {
  ports.begin();
  mux.begin();
  Screen::getInstance()->begin();
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
  ports.pinMode(PORTS_F2_HTR,OUTPUT);

  mux.pinMode(INPUT);

  pinMode(PRESSURE_1,INPUT);

  // TODO: clean up this mess:
  // IMPORTANT: To make flow sensor work these outputs have to be set to 1.
  ports.digitalWrite(PORTS_F1_PWR, 1);
  ports.digitalWrite(PORTS_F1_HTR, 1);

  restartUptime();
}

Esp32Hardware::~Esp32Hardware() {
}
