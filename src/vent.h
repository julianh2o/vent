#ifndef VENT_H
#define VENT_H

#include "hardware_interface.h"
#include "vent_machine.h"

class Vent {

 public:
  Vent(HardwareInterface* hardware);
  ~Vent();

  void tick();

 private:
  HardwareInterface& hardware_;
  VentMachine machine_;


  bool selfTestPassed_;
  double deltaT_;
};

#endif /* VENT_H */