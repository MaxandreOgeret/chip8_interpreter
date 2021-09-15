// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace mem {
  using address_t = short unsigned int;

  class Memory {
  public:
    const std::vector<uint8_t> FONT_ = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80,
            0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0,
            0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90,
            0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0,
            0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0,
            0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

    Memory();

    /**
     * @brief Sets the value at address
     * @param value
     * @param address
     */
    void poke(uint8_t value, address_t address);

    /**
     * @brief Sets the vector of values starting at address
     * @param value
     * @param address
     */
    void poke(std::vector<uint8_t> values, address_t address);

    /**
     * @brief Returns the value at address
     * @param address
     * @return
     */
    uint8_t peek(address_t address);

    /**
     * @brief Setup the pre loaded sprites http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#font
     * todo find a better place
     */
    void init();

  private:
    std::vector<uint8_t> memory;

    static inline void validate_address(address_t address) {
      if (address < 0x000) { throw std::runtime_error("Memory address < 0x000 (0)"); }
      if (address > 0xFFF) { throw std::runtime_error("Memory address > 0xFFF (4095)"); }
    }

    static inline void validate_value(uint8_t value) {
      if (value < 0x0) { throw std::runtime_error("Memory value < 0x0 (0)"); }
      if (value > 0xFF) { throw std::runtime_error("Memory value > 0xF (15)"); }
    }
  };

}// namespace mem


#endif//CHIP8_MEMORY_H
