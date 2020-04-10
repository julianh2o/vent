#include "test_suite.h"

#include "../src/fake_hardware.h"


TEST(FakeHardware, getSecondsSinceStart) {
  FakeHardware hw;

  EXPECT_EQ(hw.getSecondsSinceStart(), 0.0);
  hw.seconds_since_start = 1.0;
  EXPECT_EQ(hw.getSecondsSinceStart(), 1.0);
  hw.seconds_since_start = 2.0;
  EXPECT_EQ(hw.getSecondsSinceStart(), 2.0);
}

TEST(FakeHardware, readSensors) {
  FakeHardware hw;
  SensorState state;

  hw.read_sensors_return_value = false;
  EXPECT_FALSE(hw.readSensors(&state));

  state.p1 = 33.0;
  state.f1 = 44.0;
  hw.sensor_state.p1 = 55.0;
  hw.sensor_state.f1 = 66.0;
  hw.read_sensors_return_value = true;
  EXPECT_TRUE(hw.readSensors(&state));
  EXPECT_EQ(state.p1, hw.sensor_state.p1);
  EXPECT_EQ(state.f1, hw.sensor_state.f1);
}

TEST(FakeHardware, setValves) {
  FakeHardware hw;
  ValveState state;

  hw.set_valves_return_value = false;
  EXPECT_FALSE(hw.setValves(state));

  state.v1 = true;
  state.v2 = false;
  hw.valve_state.v1 = false;
  hw.valve_state.v2 = true;
  EXPECT_NE(state.v1, hw.valve_state.v1);
  EXPECT_NE(state.v2, hw.valve_state.v2);

  hw.set_valves_return_value = true;
  EXPECT_TRUE(hw.setValves(state));
  EXPECT_EQ(state.v1, hw.valve_state.v1);
  EXPECT_EQ(state.v2, hw.valve_state.v2);
}

TEST(FakeHardware, readControls) {
  FakeHardware hw;
  ControlState state;

  hw.read_controls_return_value = false;
  EXPECT_FALSE(hw.readControls(&state));

  state.tidal_volume = 33.0;
  hw.control_state.tidal_volume = 55.0;

  hw.read_controls_return_value = true;
  EXPECT_TRUE(hw.readControls(&state));

  EXPECT_EQ(state.tidal_volume, hw.control_state.tidal_volume);
  EXPECT_EQ(state.tidal_volume, 55.0);
}

TEST(FakeHardware, writeIndication) {
  FakeHardware hw;
  IndicationState state;

  hw.write_indication_return_value = false;
  EXPECT_FALSE(hw.writeIndication(state));

  state.status_led_mode = IndicationState::SOLID_RED;
  hw.indication_state.status_led_mode = IndicationState::SOLID_GREEN;

  hw.write_indication_return_value = true;
  EXPECT_TRUE(hw.writeIndication(state));

  EXPECT_EQ(state.status_led_mode, hw.indication_state.status_led_mode);
  EXPECT_EQ(hw.indication_state.status_led_mode, IndicationState::SOLID_RED);
}

TEST(FakeHardware, updateDisplay) {
  FakeHardware hw;
  DisplayState state;

  hw.update_display_return_value = false;
  EXPECT_FALSE(hw.updateDisplay(state));

  state.tidal_volume = 33.0;
  hw.display_state.tidal_volume = 44.0;

  hw.update_display_return_value = true;
  EXPECT_TRUE(hw.updateDisplay(state));

  EXPECT_EQ(state.tidal_volume, hw.display_state.tidal_volume);
  EXPECT_EQ(hw.display_state.tidal_volume, 33.0);
}

TEST(FakeHardware, getConfig) {
  FakeHardware hw;
  ConfigState state;

  hw.get_config_return_value = false;
  EXPECT_FALSE(hw.getConfig(&state));

  state.inhale_pressure_threshold = 33.0;
  hw.config_state.inhale_pressure_threshold = 55.0;

  hw.get_config_return_value = true;
  EXPECT_TRUE(hw.getConfig(&state));

  EXPECT_EQ(state.inhale_pressure_threshold,
            hw.config_state.inhale_pressure_threshold);
  EXPECT_EQ(state.inhale_pressure_threshold, 55.0);
}
