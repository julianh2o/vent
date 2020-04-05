#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include "time_counter.h"


struct SensorState {

  // Momentary pressure measurement provided by P1 pressure sensor, [psi].
  double p1;

  // Momentary flow rate provided by F1 flow meter, [cc per second].
  double f1;
};


struct ControlState {

  // Volume or Pressure target switch.
  enum TargetSwitchState {
    VOLUME,
    PRESSURE
  };

  // Position of Volume/Pressure Switch.
  TargetSwitchState target_switch;

  // Tidal Volume Knob Value [cc].
  double tidal_volume;

  // Inspiratory Pressure Knob Value [psi].
  double inspiratory_pressure;

  // RateAssist Switch Value. True if switch is in ON state, 
  // false if is in OFF state.
  bool rate_assist_switch;

  // Respiratory Rate Knob Value, [bpm].
  double respiratory_rate;

  // Inhale/Exhale Ratio Knob Value. Ranges approximately from 1:1 to 1:4.
  double inhale_exhale_ratio;

  // State of End Inspiratory Pause State. True if it is down, false otherwise.
  bool end_inspiratory_pause_button_down;

  // Start/Ack Button State. True if it is down, false otherwise.
  bool start_ack_button_down;
};


struct IndicationState {

  // LED is expected to work in four modes:
  enum StatusLedMode {

    // Solid RED indicates malfunction mode.
    SOLID_RED,

    // Solid GREEN indicates Ready/Standby state.
    SOLID_GREEN,

    // Blink RED indicates triggered alert.
    BLINK_RED,

    // Blink GREEN indicates normal operation in Assist Control Mode.
    BLINK_GREEN,

    // Blink YELLOW indicates that Rate Assistance is Disabled, used to 
    // execute spontaneous breathing trial.
    BLINK_YELLOW
  };

  // Status LED mode.
  StatusLedMode status_led_mode;

  // Beeper is supposed to support async alerting mode.
  enum BeeperMode {
    OFF,
    ALERT
  };

  // Beeper mode.
  BeeperMode beeper_mode;
};


struct DisplayState {

  // Volume or Pressure target switch position.
  ControlState::TargetSwitchState target;

  // Tidal Volume Knob Value [cc].
  //  - If target == VOLUME then tidal_volume is the TARGET volume.
  //  - If target == PRESSURE then tidal_volume is the ALERT volume.
  double tidal_volume;

  // Inspiratory Pressure Knob Value [psi].
  //  - If target == VOLUME then inspiratory_pressure is the ALERT pressure.
  //  - If target == PRESSURE then inspiratory_pressure is the TARGET volume.
  double inspiratory_pressure;

  // Minimal Respiratory Rate Knob Value.
  double minimum_respiratory_rate;

  // Actual (measurement) Respiratory Rate [bpm].
  double actual_respiratory_rate;

  // Actual (measurement) Tidal Volume [cc].
  double actual_tidal_volume;

  // Peak Pressure, [psi].
  double peak_pressure;

  // Peak Pressure, [psi].
  double plateau_pressure;
 
  // Peep Pressure, [psi].
  double peep_pressure;
};


struct ConfigState {
  // Inhale Pressure Threshold. It helps detect spontaneous breathing effort.
  double inhale_pressure_threshold;

  // Exhale Pressure Threshold. It helps detect spontaneous breathing effort.
  double exhale_pressure_threshold;

  // Cough Pressure Threshold. It helps detect patient cough.
  double cough_pressure_threshold;

  // Inspiratory Pause Delay. It helps measure plateau pressure, [seconds].
  double inspiratory_pause_delay_seconds;

  // Flow meter threshold to determine the absence of air flow. 
  // Small positive value, it could be slightly above ~3*sigma.
  double no_flow_threshold;

  // Inhale/exhale volume tolerance (values around ~0.95). The system uses this
  // to match inhale and exhale volumes which may not match exactly: inhale and 
  // exhale air have different ratios of oxygen and carbon dioxide.
  double volume_tolerance;
};

class HardwareInterface {

 public:
  // Returns number of seconds since unit has started (or since Unix Epoch).
  // IMPORTANT: it is important to provide fractional part as it will be used
  // for calculus. The implementation is platform dependent. See default 
  // implementation in hardware_implementation.cpp and make sure to implement it
  // for the target platform.
  virtual double getSecondsSinceStart();

  // Reads sensor values and stores them to the passed state struct. 
  // Returns true if read succeeded, false otherwise.
  virtual bool readSensors(SensorState* state)=0;

  // Reads control values and stores them to the passed state struct.
  // Returns true if read succeeded, false otherwise.
  virtual bool readControls(ControlState* state)=0;

  // Writes LED and Beeper values.
  virtual bool writeIndication(const IndicationState& state)=0;

  // Renders passed values on the LCD display.
  virtual bool updateDisplay(const DisplayState& state)=0;

  // Returns the vent configuration which contains thresholds, constants, etc.
  virtual bool getConfig(ConfigState* state)=0;

  HardwareInterface();
  virtual ~HardwareInterface();

 private:
  TimeCounter counter_;
};

#endif  // HARDWARE_INTERFACE_H