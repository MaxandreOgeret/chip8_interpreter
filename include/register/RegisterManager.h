// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_REGISTERMANAGER_H
#define CHIP8_REGISTERMANAGER_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "register/Register.h"
#include "register/Stack.h"

namespace reg {
  using regnb_t = uint8_t;

  class RegisterManager {
  public:
    RegisterManager(unsigned short int frequency);

    // 16x 8-bit general purpose registers
    std::vector<Register<uint8_t>> v_;

    // 16-bit general purpose register
    Register<uint16_t> i_;

    // 8-bit delay timer register
    Register<uint8_t> dt_;

    // 8-bit sound timer register
    Register<uint8_t> st_;

    // 16-bit program counter
    Register<uint16_t> pc_;

    // 8-bit program counter
    Register<uint8_t> sp_;

    // 16x 16-bit stack
    Stack<uint16_t> stack_ = Stack<uint16_t>(16);

    void trigger_timers();

  private:
    unsigned short int counter_ = 0;
    unsigned short int decrement_interval_;
  };
}// namespace reg
#endif//CHIP8_REGISTERMANAGER_H
