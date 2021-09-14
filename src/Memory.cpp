// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include <iostream>

#include "Memory.h"

using namespace mem;

Memory::Memory() {
  // The Chip-8 language is capable of accessing up to 4,096 bytes (0x1000) of RAM
  memory = std::vector<uint8_t>(0x1000, 0);
  init();
}

void Memory::poke(uint8_t value, address_t address) {
  validate_address(address);
  validate_value(value);
  memory[address] = value;
}

void Memory::poke(std::vector<uint8_t> values, address_t address) {
  for (int i = 0; i < values.size(); i++) { poke(values[i], address + i); }
}

uint8_t Memory::peek(address_t address) {
  validate_address(address);
  return memory[address];
}

void Memory::init() { poke(FONT_, 0x000); }
