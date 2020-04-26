#include "esp32_hardware.h"
#include "hardware_interface.h"

#include "screen.h"

const int width = 320;
const int boxHeight = 50;
const int divs = 4;
const int padding = 3;
const int boxWidth = ((width-padding) / divs) - padding;
const int columnTwo = 225;
const uint8_t boxOriginX = 0;
const uint8_t boxOriginY = 240 - boxHeight * 3;
const uint8_t boxPadding = 3;

//Perform a simple linear scale for our dials
//TODO make this an instance function?
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
  // See https://www.nxp.com/docs/en/data-sheet/MP3V5004G.pdf
  //TODO needs to be scaled by calibration values and probably the offset value
  //Let's do all of this in a lookup table
  const uint16_t pressureMax = 400; //mm H2o
  const float offsetVoltagetTypical = .6f;
  const float offset = offsetVoltagetTypical / 3.3F;
  state->P = ((float)pressureMax * (-offset + (analogRead(PRESSURE_1)/4095.0))) / 10.0;

  // flow
  state->F = flow1.read();

  return false;
}

const SensorState * Esp32Hardware::getSensorState() {
  return &sensorState;
}

void Esp32Hardware::updateSensorState() {
  readSensors(&sensorState);
}

void Esp32Hardware::setValves(bool v1, bool v2) {
  ports.digitalWrite(PORTS_24V_1,v1);
  ports.digitalWrite(PORTS_24V_2,v2);
}

bool Esp32Hardware::readControls(ControlState* state) {
  state->target_switch = ports.digitalRead(PORTS_SWITCH2) ? ControlState::TargetSwitchState::VOLUME : ControlState::TargetSwitchState::PRESSURE;
  state->Vt = normalizeDial(CONTROL_TIDAL_VOLUME_MIN,CONTROL_TIDAL_VOLUME_MAX,mux.analogRead(MUX_DIAL_4),CONTROL_TIDAL_VOLUME_DIVISIONS,CONTROL_TIDAL_VOLUME_INVERT);
  state->Pt = normalizeDial(CONTROL_INSP_PRESSURE_MIN,CONTROL_INSP_PRESSURE_MAX,mux.analogRead(MUX_DIAL_3),CONTROL_INSP_PRESSURE_DIVISIONS,CONTROL_INSP_PRESSURE_INVERT);
  state->rate_assist_switch = !ports.digitalRead(PORTS_SWITCH1);
  state->Rt = normalizeDial(CONTROL_RESP_RATE_MIN,CONTROL_RESP_RATE_MAX,mux.analogRead(MUX_DIAL_2),CONTROL_RESP_RATE_DIVISIONS,CONTROL_RESP_RATE_INVERT);
  state->Rie = normalizeDial(CONTROL_INHALE_RATIO_MIN,CONTROL_INHALE_RATIO_MAX,mux.analogRead(MUX_DIAL_1),CONTROL_INHALE_RATIO_DIVISIONS,CONTROL_INHALE_RATIO_INVERT);

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

void Esp32Hardware::setGreen(bool state) {
  ports.digitalWrite(PORTS_LED1, state);
}

void Esp32Hardware::setRed(bool state) {
  ports.digitalWrite(PORTS_LED2, state);
}

void Esp32Hardware::displayAlert(const char * newMessage) {
  buzzer.shortBeeps();
  setGreen(false);
  setRed(true);
  displayMessage(newMessage);
  messageColor = ILI9341_RED;
  forceRefresh = true;
}

void Esp32Hardware::displayMessage(const char * newMessage) {
  uint8_t size = min(MESSAGE_SIZE,(int)strlen(newMessage));
  memcpy(message,newMessage,size);
  showMessage = true;
  messageColor = ILI9341_YELLOW;
  forceRefresh = true;
}

void Esp32Hardware::standbyMode() {
  buzzer.off();
  setGreen(true);
  setRed(false);
  showMessage = false;
  forceRefresh = true;
}

//TODO add blinking lights here
void Esp32Hardware::runningMode() {
  buzzer.off();
  setGreen(true);
  setRed(false);
  showMessage = false;
  forceRefresh = true;
}

const ConfigState * Esp32Hardware::getConfig() {
  return &configState;
}

Statistics * Esp32Hardware::getStatistics() {
  return &statistics;
}

void Esp32Hardware::runTest() {
  Screen::getInstance()->runTest();
}

//This contains testing/demo behavior that is not critical to operation
void Esp32Hardware::testModeTick() {
  if (start.hasPressed()) {
    if (showMessage) {
      standbyMode();
    } else {
      displayAlert("Simulated alert");
    }
  }

  //Manually control the valve
  // if (pause.isPressed()) {
  //   setValves(controlState.rate_assist_switch,!controlState.rate_assist_switch);
  // } else {
  //   setValves(false,false);
  // }

  if (pause.hasPressed()) {
    ports.digitalWrite(PORTS_F1_PWR, controlState.rate_assist_switch);
    ports.digitalWrite(PORTS_F1_HTR, controlState.rate_assist_switch);
    delay(500);
  }
}

void Esp32Hardware::boxTextTop(uint8_t x, uint8_t y) {
    Screen::getInstance()->tft.setCursor(boxOriginX + boxPadding + x*(boxWidth + padding),boxOriginY + boxHeight * y + 3);
    Screen::getInstance()->tft.setTextSize(1);
}

void Esp32Hardware::boxTextBottom(uint8_t x, uint8_t y) {
    Screen::getInstance()->tft.setCursor(boxOriginX + boxPadding + x*(boxWidth + padding),boxOriginY + boxHeight * y + padding + 18);
    Screen::getInstance()->tft.setTextSize(3);
}

void Esp32Hardware::recordHistory() {
  unsigned long now = millis();
  if (lastHistoryRecord + HISTORY_INTERVAL < now) {
    lastHistoryRecord = now;
    newHistoryFrame = true;

    // Serial.print("before nextHisotryIndex: ");
    // Serial.print(nextHistoryIndex);
    // Serial.print(" firstHistoryIndex: ");
    // Serial.print(firstHistoryIndex);
    // Serial.println();

    SensorState * historyItem = &sensorHistory[nextHistoryIndex];

    historyItem->P = sensorState.P;
    historyItem->F = sensorState.F;

    nextHistoryIndex ++;
    if (nextHistoryIndex > HISTORY_SIZE) nextHistoryIndex = 0;
    if (firstHistoryIndex == nextHistoryIndex) firstHistoryIndex ++;

    // Serial.print("before nextHisotryIndex: ");
    // Serial.print(nextHistoryIndex);
    // Serial.print(" firstHistoryIndex: ");
    // Serial.print(firstHistoryIndex);
    // Serial.println();
  }
}

void Esp32Hardware::tick() {
  Screen* screen = Screen::getInstance();
  buzzer.tick();
  updateControlState();
  updateSensorState();

  recordHistory();

  bool refresh = false;
  if (forceRefresh || controlState.target_switch != lastControlState.target_switch || controlState.rate_assist_switch != lastControlState.rate_assist_switch) {
    refresh = true;
    forceRefresh = false;
    screen->clear();
  }

  screen->tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  screen->tft.setTextSize(2);

  // uint8_t temp = normalizeDial(0,300,mux.analogRead(MUX_DIAL_3),1,true); //can use this for testing gfx positioning..
  uint8_t lineHeight = 18;

  if (controlState.target_switch == ControlState::VOLUME) {
    if (refresh) {
      screen->tft.setCursor(0, 0);
      screen->tft.print("Mode: ACV Target Volume");
      //DEBUGGING
      // screen->tft.print(temp);
    }

    if (refresh || controlState.Vt != lastControlState.Vt) {
      screen->tft.setCursor(0, lineHeight);
      screen->tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      screen->tft.print("Target Volume: ");
      screen->tft.setCursor(columnTwo, lineHeight);
      screen->padprint("%.0f cc",controlState.Vt,7);
    }

    if (refresh || controlState.Pt != lastControlState.Pt) {
      screen->tft.setCursor(0, 2*lineHeight);
      screen->tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      screen->tft.print("Alert Pressure: ");
      screen->tft.setCursor(columnTwo, 2*lineHeight);
      screen->padprint("%.2f cm",controlState.Pt,7);
    }
  } else {
    if (refresh) {
      screen->tft.setCursor(0, 0);
      screen->tft.println("Mode: ACV Target Pressure");
    }

    if (refresh || controlState.Pt != lastControlState.Pt) {
      screen->tft.setCursor(0, lineHeight);
      screen->tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      screen->tft.print("Target Pressure: ");
      screen->tft.setCursor(columnTwo, lineHeight);
      screen->padprint("%.2f cm",controlState.Pt,7);
      screen->tft.println();
    }

    if (refresh || controlState.Vt != lastControlState.Vt) {
      screen->tft.setCursor(0, 2*lineHeight);
      screen->tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      screen->tft.print("Alert Volume: ");
      screen->tft.setCursor(columnTwo, 2*lineHeight);
      screen->padprint("%.0f cc",controlState.Vt,7);
      screen->tft.println();
    }
  }

  screen->tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

  if (controlState.rate_assist_switch) {
    if (refresh || controlState.Rt != lastControlState.Rt) {
      screen->tft.setCursor(0, 3*lineHeight);
      screen->tft.print("Min resp rate: ");
      screen->tft.setCursor(columnTwo, 3*lineHeight);
      screen->padprint("%.0f bpm",controlState.Rt,6);
      screen->tft.println();
    }

    if (refresh || controlState.Rie != lastControlState.Rie) {
      screen->tft.setCursor(0, 4*lineHeight);
      screen->tft.print("Resp Ratio:");
      screen->tft.setCursor(columnTwo, 4*lineHeight);
      screen->tft.print("1:");
      screen->padprint("%.2f",controlState.Rie,9);
      screen->tft.println();
    }
  } else {
    if (refresh) {
      screen->tft.setCursor(0, 3*lineHeight);
      screen->tft.println("Rate assist OFF");
    }
  }

  if (refresh) {
    for (int i=0; i<divs; i++) {
      screen->tft.fillRect(0, 240 - (boxHeight * (3-i)), boxWidth, boxHeight-1, ILI9341_BLACK);
      screen->tft.drawRect(0, 240 - (boxHeight * (3-i)), boxWidth, boxHeight-1, ILI9341_CYAN);
    }
  }

  if (refresh || sensorState.P != lastSensorState.P) {
    boxTextTop(0,0);
    screen->tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    screen->tft.print("Press [cc]");
    boxTextBottom(0,0);
    screen->padprint("%.1f",sensorState.P,4);
  }

  if (refresh || sensorState.F != lastSensorState.F) {
    boxTextTop(0,1);
    screen->tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    screen->tft.print("Flow [cm]");
    boxTextBottom(0,1);
    if (sensorState.F == -1) {
      screen->tft.print("ERR");
    } else {
      screen->padprint("%.0f",sensorState.F,4);
    }
  }

  if (refresh && showMessage) {
    screen->tft.setTextColor(messageColor, ILI9341_BLACK);
    screen->tft.setCursor(0, 170);
    screen->tft.print("Msg: ");
    screen->tft.print(message);
  }

  screen->tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

  //width=320, height=240
  if (refresh) {
    for (int i=0; i<divs; i++) {
      screen->tft.fillRect(i*(boxWidth+padding), 240-boxHeight, boxWidth, boxHeight-1, ILI9341_BLACK);
      screen->tft.drawRect(i*(boxWidth+padding), 240-boxHeight, boxWidth, boxHeight-1, ILI9341_WHITE);
    }
  }

  //Tidal volume
  if (refresh || controlState.Vt != lastControlState.Vt) {
    boxTextTop(0,2);
    screen->tft.print("TVol [cc]");
    boxTextBottom(0,2);
    screen->padprint("%.0f",controlState.Vt,4);
  }

  //Insp pressure
  if (refresh || controlState.Pt != lastControlState.Pt) {
    boxTextTop(1,2);
    screen->tft.print("Insp [cm]");
    boxTextBottom(1,2);
    screen->padprint("%.2f",controlState.Pt,4);
  }

  //Resp rate
  if (refresh || controlState.Rt != lastControlState.Rt) {
    screen->tft.setTextColor(controlState.rate_assist_switch ? ILI9341_WHITE : ILI9341_DARKGREY, ILI9341_BLACK);
    boxTextTop(2,2);
    screen->tft.print("Rate [bpm]");

    boxTextBottom(2,2);
    screen->padprint("%.0f",controlState.Rt,2);
  }

  //Inhale/Exhale Ratio
  if (refresh || controlState.Rie != lastControlState.Rie) {
    screen->tft.setTextColor(controlState.rate_assist_switch ? ILI9341_WHITE : ILI9341_DARKGREY, ILI9341_BLACK);
    boxTextTop(3,2);
    screen->tft.print("in/ex ratio");

    boxTextBottom(3,2);
    screen->padprint("%.2f",controlState.Rie,4);
  }

  drawChart(refresh);

  lastControlState.target_switch = controlState.target_switch;
  lastControlState.Vt = controlState.Vt;
  lastControlState.Pt = controlState.Pt;
  lastControlState.rate_assist_switch = controlState.rate_assist_switch;
  lastControlState.Rt = controlState.Rt;
  lastControlState.Rie = controlState.Rie;
  lastControlState.end_inspiratory_pause_button_down = controlState.end_inspiratory_pause_button_down;
  lastControlState.start_ack_button_down = controlState.start_ack_button_down;

  lastSensorState.P = sensorState.P;
  lastSensorState.F = sensorState.F;
}

void Esp32Hardware::drawChart(bool refresh) {
  Screen* screen = Screen::getInstance();
  const uint16_t chartOffsetX = boxWidth+3;
  const uint16_t chartOffsetY = 240 - boxHeight * 3;
  const uint16_t chartHeight = boxHeight * 2 - 3;
  const uint16_t chartWidth = min(HISTORY_SIZE,320-chartOffsetX);

  if (refresh) {
    screen->tft.fillRect(chartOffsetX,chartOffsetY, chartWidth, chartHeight, ILI9341_BLACK);
    screen->tft.drawRect(chartOffsetX-1,chartOffsetY-1, chartWidth+2, chartHeight+2, ILI9341_WHITE);
  }

  screen->tft.startWrite();
  for (uint16_t i=0; i<HISTORY_SIZE; i++) {
    if (!refresh && i != nextHistoryIndex-1 && !newHistoryFrame) continue;
    if (i == 0) continue;

    screen->tft.writeFastVLine(chartOffsetX+i-1,chartOffsetY,chartHeight,ILI9341_BLACK);
    screen->tft.writeFastVLine(chartOffsetX+i,chartOffsetY,chartHeight,ILI9341_BLACK);

    uint16_t plotPp = (40.0/chartHeight) * sensorHistory[i-1].P;
    uint16_t plotP = (40.0/chartHeight) * sensorHistory[i].P;
    // Serial.print("plot: ");
    // Serial.print(i);
    // Serial.print(" ");
    // Serial.print(plotP);
    // Serial.println();
    screen->tft.writeLine(chartOffsetX+i-1,chartOffsetY+chartHeight-plotPp,chartOffsetX+i,chartOffsetY+chartHeight-plotP,ILI9341_WHITE);
    if (!refresh) newHistoryFrame = false;
  }
  screen->tft.endWrite();
}

Esp32Hardware::Esp32Hardware() : HardwareInterface(),
  ports(I2C_1_SDA,I2C_1_SCL,IO_RST),
  mux(MUX0,MUX1,MUX2,MUXIO),
  buzzer(BUZZER_PIN,0),
  flow1() {

  lastHistoryRecord = 0;
  nextHistoryIndex = 0;
  firstHistoryIndex = 0;

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
  ports.digitalWrite(PORTS_F1_PWR, 0);
  ports.digitalWrite(PORTS_F1_HTR, 0);
  delay(200);
  ports.digitalWrite(PORTS_F1_PWR, 1);
  ports.digitalWrite(PORTS_F1_HTR, 1);

  restartUptime();
}

Esp32Hardware::~Esp32Hardware() {
}
