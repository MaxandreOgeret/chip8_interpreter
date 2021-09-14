// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_ROMPARSER_H
#define CHIP8_ROMPARSER_H

#include "Memory.h"
#include "Instructions.h"
#include "register/RegisterManager.h"

#include "iostream"
#include <fstream>
#include <memory>
#include <vector>
#include <exception>
#include <sstream>

class RomParser {
public:
  explicit RomParser(std::string romPath, std::shared_ptr<mem::Memory> memory, std::shared_ptr<reg::RegisterManager> registerManager, std::shared_ptr<Instructions> instructions);
  std::shared_ptr<mem::Memory> memory_;
  std::shared_ptr<reg::RegisterManager> registers_;
  std::shared_ptr<Instructions> instructions_;

  void step();
  void decode();
  uint16_t get_from_opcode(const uint16_t &opcode, const uint16_t mask);
  /**
   * For testing/debugging
   * @param opcode
   */
  void set_opcode(uint16_t opcode);

private:
  std::string romPath_;
  std::ifstream source_;
  std::vector<uint8_t> contents_;
  std::vector<uint8_t> opcode_encoded_{0x0, 0x0};
  uint16_t opcode_;
  std::stringstream ss;
};


#endif//CHIP8_ROMPARSER_H
