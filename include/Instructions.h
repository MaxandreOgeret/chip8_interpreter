// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_INSTRUCTIONS_H
#define CHIP8_INSTRUCTIONS_H

#include <memory>
#include <random>
#include <iostream>

#include "Interface.h"
#include "Memory.h"
#include "register/RegisterManager.h"

using namespace mem;
using namespace reg;

class Instructions {
public:
  Instructions(const std::shared_ptr<mem::Memory> &memory, const std::shared_ptr<reg::RegisterManager> &registers, const std::shared_ptr<Interface> & interface);

private:
  std::shared_ptr<mem::Memory> memory_;
  std::shared_ptr<reg::RegisterManager> registers_;
  std::shared_ptr<Interface> interface_;

  std::random_device rd;
  std::mt19937 mt;
  std::unique_ptr<std::uniform_int_distribution<uint8_t>> randbyte;

  // Temporary variables used by instructions
  int x_, y_;
  uint8_t sprite_byte_;
  bool sprite_pixel_is_on_, disp_pixel_is_on_;

public:
  void sys_0nnn(address_t addr);

  /**
   * 00E0 - CLS
   * Clear the display.
   */
  void cls_00E0();

  /**
   * 00EE - RET
   * Return from a subroutine.
   * The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
   */
  void ret_00EE();

  /**
   * 1nnn - JP addr
   * Jump to location nnn.
   * The interpreter sets the program counter to nnn.
   * @param addr
   */
  void jp_1nnn(address_t addr);

  /**
   * 2nnn - CALL addr
   * Call subroutine at nnn.
   * The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
   * @param addr
   */
  void call_2nnn(address_t addr);

  /**
   *  3xkk - SE Vx, byte
   *  Skip next instruction if Vx = kk.
   *  The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
   * @param vx
   * @param byte
   */
  void se_3xkk(regnb_t vx, uint8_t byte);

  /**
   *  4xkk - SNE Vx, byte
   *  Skip next instruction if Vx != kk.
   *  The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
   * @param vx
   * @param byte
   */
  void sne_4xkk(regnb_t vx, uint8_t byte);

  /**
   *  5xy0 - SE Vx, Vy
   *  Skip next instruction if Vx = Vy.
   *  The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
   * @param vx
   * @param kk
   */
  void se_5xy0(regnb_t vx, regnb_t vy);

  /**
   * 6xkk - LD Vx, byte
   * Set Vx = kk.
   * The interpreter puts the value kk into register Vx.
   * @param vx
   * @param byte
   */
  void ld_6xkk(regnb_t vx, uint8_t byte);

  /**
   * 7xkk - ADD Vx, byte
   * Set Vx = Vx + kk.
   * Adds the value kk to the value of register Vx, then stores the result in Vx.
   * @param vx
   * @param byte
   */
  void add_7xkk(regnb_t vx, uint8_t byte);

  /**
   * 8xy0 - LD Vx, Vy
   * Set Vx = Vy.
   * Stores the value of register Vy in register Vx.
   * @param vx
   * @param vy
   */
  void ld_8xy0(regnb_t vx, regnb_t vy);

  /**
   *  8xy1 - OR Vx, Vy
   *  Set Vx = Vx OR Vy.
   *  Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
   * @param vx
   * @param vy
   */
  void or_8xy1(regnb_t vx, regnb_t vy);

  /**
   * 8xy2 - AND Vx, Vy
   * Set Vx = Vx AND Vy.
   * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
   * @param vx
   * @param vy
   */
  void and_8xy2(regnb_t vx, regnb_t vy);

  /**
   * 8xy3 - XOR Vx, Vy
   * Set Vx = Vx XOR Vy.
   * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
   * @param vx
   * @param vy
   */
  void xor_8xy3(regnb_t vx, regnb_t vy);

  /**
   * 8xy4 - ADD Vx, Vy
   * Set Vx = Vx + Vy, set VF = carry.
   * The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
   * @param vx
   * @param vy
   */
  void add_8xy4(regnb_t vx, regnb_t vy);

  /**
   * 8xy5 - SUB Vx, Vy
   * Set Vx = Vx - Vy, set VF = NOT borrow.
   * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
   * @param vx
   * @param vy
   */
  void sub_8xy5(regnb_t vx, regnb_t vy);

  /**
   * 8xy6 - SHR Vx {, Vy}
   * Set Vx = Vx SHR 1.
   * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is bit shifted right by 1.
   * @param vx
   * @param vy
   */
  void shr_8xy6(regnb_t vx, regnb_t vy = 0x0);

  /**
   *  8xy7 - SUBN Vx, Vy
   *  Set Vx = Vy - Vx, set VF = NOT borrow.
   *  If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
   * @param vx
   * @param vy
   */
  void subn_8xy7(regnb_t vx, regnb_t vy);

  /**
   * 8xyE - SHL Vx {, Vy}
   * Set Vx = Vx SHL 1.
   * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
   * @param vx
   * @param vy
   */
  void shl_8xyE(regnb_t vx, regnb_t vy = 0x0);

  /**
   * 9xy0 - SNE Vx, Vy
   * Skip next instruction if Vx != Vy.
   * The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
   * @param vx
   * @param vy
   */
  void sne_9xy0(regnb_t vx, regnb_t vy);

  /**
   * Annn - LD I, addr
   * Set I = nnn.
   * The value of register I is set to nnn.
   * @param vx
   * @param vy
   */
  void ld_Annn(address_t addr);

  /**
   * Bnnn - JP V0, addr
   * Jump to location nnn + V0.
   * The program counter is set to nnn plus the value of V0.
   * @param addr
   */
  void jp_Bnnn(address_t addr);

  /**
   * Cxkk - RND Vx, byte
   * Set Vx = random byte AND kk.
   * The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
   * @param vx
   * @param byte
   */
  void rnd_Cxkk(regnb_t vx, uint8_t byte);

  /**
   * Dxyn - DRW Vx, Vy, nibble
   * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
   * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
   * @param vx
   * @param vy
   * @param n
   */
  void drw_Dxyn(regnb_t vx, regnb_t vy, uint8_t n);

  /**
   * Ex9E - SKP Vx
   * Skip next instruction if key with the value of Vx is pressed.
   * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
   * @param vx
   */
  void skp_Ex9E(regnb_t vx);

  /**
   * ExA1 - SKNP Vx
   * Skip next instruction if key with the value of Vx is not pressed.
   * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
   * @param vx
   */
  void sknp_ExA1(regnb_t vx);

  /**
   * Fx07 - LD Vx, DT
   * Set Vx = delay timer value.
   * The value of DT is placed into Vx.
   * @param vx
   */
  void ld_Fx07(regnb_t vx);

  /**
   * Fx0A - LD Vx, K
   * Wait for a key press, store the value of the key in Vx.
   * All execution stops until a key is pressed, then the value of that key is stored in Vx.
   * @param vx
   */
  void ld_Fx0A(regnb_t vx);// todo input

  /**
   * Fx15 - LD DT, Vx
   * Set delay timer = Vx.
   * DT is set equal to the value of Vx.
   * @param vx
   */
  void ld_Fx15(regnb_t vx);

  /**
   * Fx18 - LD ST, Vx
   * Set sound timer = Vx.
   * ST is set equal to the value of Vx.
   * @param vx
   */
  void ld_Fx18(regnb_t vx);

  /**
   * Fx1E - ADD I, Vx
   * Set I = I + Vx.
   * The values of I and Vx are added, and the results are stored in I.
   * @param vx
   */
  void add_Fx1E(regnb_t vx);

  /**
   * Fx29 - LD F, Vx
   * Set I = location of sprite for digit Vx.
   * The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
   * @param vx
   */
  void ld_Fx29(regnb_t vx);

  /**
   * Fx33 - LD B, Vx
   * Store BCD representation of Vx in memory locations I, I+1, and I+2.
   * The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
   * @param vx
   */
  void ld_Fx33(regnb_t vx);

  /**
   * Fx55 - LD [I], Vx
   * Store registers V0 through Vx in memory starting at location I.
   * The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
   * @param vx
   */
  void ld_Fx55(regnb_t vx);

  /**
   * Fx65 - LD Vx, [I]
   * Read registers V0 through Vx from memory starting at location I.
   * The interpreter reads values from memory starting at location I into registers V0 through Vx.
   * @param vx
   */
  void ld_Fx65(regnb_t vx);
};


#endif//CHIP8_INSTRUCTIONS_H
