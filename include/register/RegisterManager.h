// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_REGISTERMANAGER_H
#define CHIP8_REGISTERMANAGER_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <vector>

#include "register/Register.h"

namespace reg {
  using regnb_t = uint8_t;

  class RegisterManager {
  public:
    RegisterManager(unsigned short int frequency);

    // 16x 8-bit general purpose registers
    std::array<Register<uint8_t>, 0x10> v_;

    // 16-bit general purpose register
    Register<uint16_t> i_;

    // 8-bit delay timer register
    Register<uint8_t> dt_;

    // 8-bit sound timer register
    Register<uint8_t> st_;

    // 16-bit program counter
    Register<uint16_t> pc_;

    // 16x 16-bit stack
    std::stack<uint16_t> stack_;

    /**
     * Decrements the timers at a fixed 60Hz frequency.
     */
    void trigger_timers();

  private:
    unsigned short int counter_ = 0;
    unsigned short int decrement_interval_;
  };
}// namespace reg
#endif//CHIP8_REGISTERMANAGER_H
