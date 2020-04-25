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

  state.P = 33.0;
  state.F = 44.0;
  hw.sensor_state.P = 55.0;
  hw.sensor_state.F = 66.0;
  hw.read_sensors_return_value = true;
  EXPECT_TRUE(hw.readSensors(&state));
  EXPECT_EQ(state.P, hw.sensor_state.P);
  EXPECT_EQ(state.F, hw.sensor_state.F);
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

  state.Vt = 33.0;
  hw.control_state.Vt = 55.0;

  hw.read_controls_return_value = true;
  EXPECT_TRUE(hw.readControls(&state));

  EXPECT_EQ(state.Vt, hw.control_state.Vt);
  EXPECT_EQ(state.Vt, 55.0);
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
  Statistics state;

  hw.update_display_return_value = false;
  EXPECT_FALSE(hw.updateDisplay(state));

  state.Vt = 33.0;
  hw.display_state.Vt = 44.0;

  hw.update_display_return_value = true;
  EXPECT_TRUE(hw.updateDisplay(state));

  EXPECT_EQ(state.Vt, hw.display_state.Vt);
  EXPECT_EQ(hw.display_state.Vt, 33.0);
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
