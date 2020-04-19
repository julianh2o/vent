#include "hardware_interface.h"

HardwareInterface::HardwareInterface() {
}

HardwareInterface::~HardwareInterface() {
}

double HardwareInterface::getSecondsSinceStart() {
  return counter_.getSecondsSinceCreation();
}
