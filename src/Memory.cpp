// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include <iostream>

#include "Memory.h"

using namespace mem;

Memory::Memory() {
  memory_.fill(0x0);
  init();
}

void Memory::poke(uint8_t value, address_t address) {
  validate_address(address);
  validate_value(value);
  memory_[address] = value;
}

void Memory::poke(std::vector<uint8_t> values, address_t address) {
  for (int i = 0; i < values.size(); i++) { poke(values[i], address + i); }
}

uint8_t Memory::peek(address_t address) {
  validate_address(address);
  return memory_[address];
}

void Memory::init() {
  poke(FONT_, 0x000);
}
