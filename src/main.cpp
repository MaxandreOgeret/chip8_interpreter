// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include <iostream>
#include <memory>

#include "Emulator.h"
//#include "Memory.h"
//#include "register/RegisterManager.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cout << "Please give path to ROM." << std::endl;
    return 1;
  }

  std::unique_ptr<Emulator> emulator = std::make_unique<Emulator>(argv[1]);
  emulator->loop();

  return 0;
}
