// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include <memory>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "Instructions.h"
#include "Interface.h"
#include "Memory.h"
#include "RomParser.h"
#include "register/RegisterManager.h"

class Emulator {
public:
  Emulator(std::string romPath);
  void loop();

private:
  std::shared_ptr<mem::Memory> memory_;
  std::shared_ptr<reg::RegisterManager> registers_;
  std::shared_ptr<Instructions> instructions_;
  std::shared_ptr<Interface> interface_;
  std::shared_ptr<RomParser> romParser_;
};


#endif//CHIP8_EMULATOR_H
