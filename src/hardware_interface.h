#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

//
// Sensor Readings.
//
struct SensorState {
  // Momentary pressure measurement provided by P1 pressure sensor, [psi].
  double P;

  // Momentary flow rate provided by F1 flow meter, [cc per second].
  double F;
};

//
// Valve states.
//
struct ValveState {

  // State of the V1 valve. True - open, False - Closed.
  bool v1;

  // State of the V2 valve. True - open, False - Closed.
  bool v2;
};

//
// Values set by knobs and switches on the Control Panel.
//
struct ControlState {

  // Volume or Pressure target switch.
  enum TargetSwitchState {
    VOLUME,
    PRESSURE
  };

  // Position of Volume/Pressure Switch.
  TargetSwitchState target_switch;

  // Tidal Volume Knob Value [cc].
  double Vt;

  // Inspiratory Pressure Knob Value [psi].
  double Pt;

  // RateAssist Switch Value. True if switch is in ON state,
  // false if is in OFF state.
  bool rate_assist_switch;

  // Respiratory Rate Knob Value, [bpm].
  double Rt;

  // Inhale/Exhale Ratio Knob Value. Ranges approximately from 1:1 to 1:4.
  double Rie;

  // State of End Inspiratory Pause State. True if it is down, false otherwise.
  bool end_inspiratory_pause_button_down;

  // Start/Ack Button State. True if it is down, false otherwise.
  bool start_ack_button_down;
};


//
// Status LED and Beeper states.
//
struct IndicationState {

  // LED is expected to work in four modes:
  enum StatusLedMode {

    // Solid GREEN indicates Ready/Standby state.
    SOLID_GREEN,

    // Solid RED indicates malfunction mode.
    SOLID_RED,

    // Blink RED indicates triggered alert.
    //BLINK_RED,

    // Blink GREEN indicates normal operation in Assist Control Mode.
    //BLINK_GREEN
  };

  // Status LED mode.
  StatusLedMode status_led_mode;

  // Beeper is supposed to support async alerting mode.
  enum BeeperMode {
    OFF,
    SHORT_BEEPS,
    LONG_BEEPS
  };

  // Beeper mode.
  BeeperMode beeper_mode;
};


//
// Information that need to be rendered on the embedded LCD display.
//
struct Statistics {
  // Minimal Respiratory Rate Knob Value.
  double minimum_Rt;

  // Actual (measurement) Respiratory Rate [bpm].
  double actual_Rt;

  // Actual (measurement) Tidal Volume [cc].
  double actual_Vt;

  // Peak Pressure, [psi].
  double peak_pressure;

  // Peak Pressure, [psi].
  double plateau_pressure;

  // Peep Pressure, [psi].
  double peep_pressure;
};


//
// The vent configuration. Contains pressure thresholds and constants.
//
struct ConfigState {
  // Inhale Pressure Threshold. It helps detect spontaneous breathing effort.
  double p_in;

  // Exhale Pressure Threshold. It helps detect spontaneous breathing effort.
  double p_ex;

  // Target expiration length [seconds].
  double t_tex;

  // A flow meter threshold to determine absence of air flow, small positive value (~3σ);
  double f_stop;

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


//
// Hardware interface protocols the interaction between the vent hardware
// components (sensors, vents and controls), display and vent configuration.
//
class HardwareInterface {

 public:
  // Returns number of seconds since unit has started (or since Unix Epoch).
  // IMPORTANT: it is important to provide fractional part as it will be used
  // for calculus. The implementation is platform dependent. See default
  // implementation in hardware_implementation.cpp and make sure to implement it
  // for the target platform.
  virtual double getSecondsSinceStart()=0;

  // gets the pointer to the current sensor state
  virtual const SensorState * getSensorState()=0;

  // gets the pointer to the current control state
  virtual const ControlState* getControlState()=0;

  // Returns the vent configuration which contains thresholds, constants, etc.
  virtual const ConfigState* getConfig()=0;

  // Information that is to be logged or displayed
  // This struct can be modified to represent changes in the statistics
  virtual Statistics * getStatistics()=0;

  // Controls valves. Opens or closes valves according to the passed state.
  virtual void setValves(bool v1, bool v2)=0;

  // Display messages to the screen and call attention using the buzzer
  virtual void displayAlert(const char *)=0;
  virtual void displayMessage(const char *)=0;

  virtual void standbyMode()=0;
  virtual void runningMode()=0;

  // This function needs to be called periodically to perform various hardware
  // functionalities.
  virtual void tick()=0;

  HardwareInterface();
  virtual ~HardwareInterface();
};

#endif  // HARDWARE_INTERFACE_H
