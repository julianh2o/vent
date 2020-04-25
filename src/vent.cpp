#include "vent.h"

#include <Arduino.h>
#include <map>
#include <string>

//
// Vent Machine States, Rev 2
//
#define POWER_ON_STATE 1
#define SELF_TEST_STATE 2
#define MALFUNCTION_STATE 3
#define STANDBY_STATE 4
#define LEAD_UP_PAUSE_STATE 5
#define LEAD_UP_EXPIRATION_STATE 6
#define APNEA1_WARNING_STATE 7
#define INSPIRATION_BEGIN_STATE 8
#define RATE_CHECK_STATE 9
#define VOLUME_MISMATCH_ALERT_STATE 10
#define INSPIRATION_PROCESS_STATE 11
#define EXPIRATION_END_STATE 12
#define HIGH_INSPIRATORY_PRESSURE_ALERT_STATE 13
#define COUGH_STATE 14
#define APNEA2_WARNING_STATE 15
#define INSPIRATION_END_STATE 16
#define EXPIRATION_PAUSE_STATE 17
#define LOW_TIDAL_VOLUME_ALERT_STATE 18
#define END_INSPIRATORY_CHECK_STATE 19
#define END_INSPIRATORY_PAUSE_STATE 20
#define INSPIRATION_PAUSE_STATE 21
#define APNEA3_WARNING_STATE 22
#define EXPIRATION_PROCESS_STATE 23
#define EXPIRATION_BEGIN_STATE 24

#define DUMMY_STATE 25

static std::map<int, std::string> getStateName = {
  { POWER_ON_STATE, "POWER_ON_STATE" },
  { SELF_TEST_STATE, "SELF_TEST_STATE" },
  { MALFUNCTION_STATE, "MALFUNCTION_STATE" },
  { STANDBY_STATE, "STANDBY_STATE" },
  { LEAD_UP_PAUSE_STATE, "LEAD_UP_PAUSE_STATE" },
  { LEAD_UP_EXPIRATION_STATE, "LEAD_UP_EXPIRATION_STATE" },
  { APNEA1_WARNING_STATE, "APNEA1_WARNING_STATE" },
  { INSPIRATION_BEGIN_STATE, "INSPIRATION_BEGIN_STATE" },
  { RATE_CHECK_STATE, "RATE_CHECK_STATE" },
  { VOLUME_MISMATCH_ALERT_STATE, "VOLUME_MISMATCH_ALERT_STATE" },
  { INSPIRATION_PROCESS_STATE, "INSPIRATION_PROCESS_STATE" },
  { EXPIRATION_END_STATE, "EXPIRATION_END_STATE" },
  { HIGH_INSPIRATORY_PRESSURE_ALERT_STATE,
    "HIGH_INSPIRATORY_PRESSURE_ALERT_STATE" },
  { COUGH_STATE, "COUGH_STATE" },
  { APNEA2_WARNING_STATE, "APNEA2_WARNING_STATE" },
  { INSPIRATION_END_STATE, "INSPIRATION_END_STATE" },
  { EXPIRATION_PAUSE_STATE, "EXPIRATION_PAUSE_STATE" },
  { LOW_TIDAL_VOLUME_ALERT_STATE, "LOW_TIDAL_VOLUME_ALERT_STATE" },
  { END_INSPIRATORY_CHECK_STATE, "END_INSPIRATORY_CHECK_STATE" },
  { END_INSPIRATORY_PAUSE_STATE, "END_INSPIRATORY_PAUSE_STATE" },
  { INSPIRATION_PAUSE_STATE, "INSPIRATION_PAUSE_STATE" },
  { APNEA3_WARNING_STATE, "APNEA3_WARNING_STATE" },
  { EXPIRATION_PROCESS_STATE, "EXPIRATION_PROCESS_STATE" },
  { EXPIRATION_BEGIN_STATE, "EXPIRATION_BEGIN_STATE" },
};


Vent::Vent(HardwareInterface* hardware) : hardware_(*hardware) {

  selfTestPassed_ = false;

  //
  // Power On State:
  //
  machine_.addState(POWER_ON_STATE, []() {
  }).addExit(STANDBY_STATE, []() {
    return true;
  });

  //
  // Standby State.
  //
  machine_.addState(STANDBY_STATE, [&]() {
    hardware_.standbyMode();
  }).addExit(INSPIRATION_BEGIN_STATE, [&]() {
    return (((hardware_.getSecondsSinceStart() - deltaT_) > 1.5) && 
      c->start_ack_button_down);
  });

  //
  // Simplified inspiration cycle.
  //
  machine_.addState(INSPIRATION_BEGIN_STATE, [&]() {
    hardware_.setValves(true, false);
  }).addExit(EXPIRATION_BEGIN_STATE, [&]() {
    return ((hardware_.getSecondsSinceStart() - deltaT_) > 1.0);
  }).addExit(COUGH_STATE, [&]() {
    return s.P > 4000;
  });

  machine_.addState(EXPIRATION_BEGIN_STATE, [&]() {
    hardware_.setValves(false, true);
  }).addExit(INSPIRATION_BEGIN_STATE, [&]() {
    return ((hardware_.getSecondsSinceStart() - deltaT_) > 2.0);
  }).addExit(STANDBY_STATE, [&]() {
    return c->start_ack_button_down;
  });

  //
  // Cough state.
  //
  machine_.addState(COUGH_STATE, []() {
  }).addExit(INSPIRATION_BEGIN_STATE, []() {
    return ((hardware_.getSecondsSinceStart() - deltaT_) > 5.0);
  });

  machine_.addState(RATE_CHECK_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(VOLUME_MISMATCH_ALERT_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(INSPIRATION_PROCESS_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(EXPIRATION_END_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(HIGH_INSPIRATORY_PRESSURE_ALERT_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });

  machine_.addState(APNEA2_WARNING_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(INSPIRATION_END_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(EXPIRATION_PAUSE_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(LOW_TIDAL_VOLUME_ALERT_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(END_INSPIRATORY_CHECK_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(END_INSPIRATORY_PAUSE_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(INSPIRATION_PAUSE_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(APNEA3_WARNING_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });
  machine_.addState(EXPIRATION_PROCESS_STATE, []() {
  }).addExit(DUMMY_STATE, []() {
    return true;
  });



  // Start from Power On State.
  machine_.setCurrentState(POWER_ON_STATE);
}

Vent::~Vent() {
}

void Vent::tick() {
  hardware_.tick();

  //Actually not necessary to call these every loop.... the references are static
  s = hardware_.getSensorState();
  c = hardware_.getControlState();
  config = hardware_.getConfig();

  int prev_state = machine_.getCurrentStateId();
  int new_state = machine_.tick();
  if (new_state != prev_state) {
    deltaT_ = hardware_.getSecondsSinceStart();

    Serial.print(getStateName[prev_state].c_str());
    Serial.print(" -> ");
    Serial.print(getStateName[new_state].c_str());
    Serial.print("\n");
  }
}
