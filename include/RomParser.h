// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_ROMPARSER_H
#define CHIP8_ROMPARSER_H

#include "Configuration.h"
#include "Instructions.h"
#include "Memory.h"
#include "register/RegisterManager.h"

#include "iostream"
#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

class RomParser {
public:
  explicit RomParser(const std::shared_ptr<Configuration> configuration,
                     const std::shared_ptr<mem::Memory> memory,
                     const std::shared_ptr<reg::RegisterManager> registerManager,
                     const std::shared_ptr<Instructions> instructions);
  std::shared_ptr<Configuration> configuration_;
  std::shared_ptr<mem::Memory> memory_;
  std::shared_ptr<reg::RegisterManager> registers_;
  std::shared_ptr<Instructions> instructions_;

  /**
   * Goes one step forward in the program execution.
   * Loads OPCODE and increments PC.
   */
  void step();

  /**
   * Decodes the OPCODE and calls the corresponding instruction.
   * @throws std::runtime_error if OPCODE is unknown
   */
  void decode();

  /**
   * Extracts the values from the OPCODE.
   * @param opcode
   * @param mask
   * @return
   */
  uint16_t get_from_opcode(const uint16_t & opcode, const uint16_t mask);

  /**
   * For testing/debugging
   * @param opcode
   */
  void set_opcode(uint16_t opcode);

private:
  std::ifstream source_;
  std::vector<uint8_t> contents_;
  std::vector<uint8_t> opcode_encoded_{0x0, 0x0};
  uint16_t opcode_;
  std::stringstream ss;
};


#endif//CHIP8_ROMPARSER_H
