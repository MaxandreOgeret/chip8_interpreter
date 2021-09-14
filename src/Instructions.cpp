// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Instructions.h"

Instructions::Instructions(const std::shared_ptr<mem::Memory> & memory,
                           const std::shared_ptr<reg::RegisterManager> & registers,
                           const std::shared_ptr<Interface> & interface)
    : memory_(memory), registers_(registers), interface_(interface) {
  mt = std::mt19937(rd());
  randbyte = std::make_unique<std::uniform_int_distribution<uint8_t>>(0, 255);
}

void Instructions::sys_0nnn(address_t addr) {}

void Instructions::cls_00E0() { interface_->clear(); }

void Instructions::ret_00EE() {
  registers_->pc_.poke(registers_->stack_.pop_back());
  registers_->sp_.decrement();
}

void Instructions::jp_1nnn(address_t addr) { registers_->pc_.poke(addr); }

void Instructions::call_2nnn(address_t addr) {
  registers_->sp_.increment();
  registers_->stack_.push_back(registers_->pc_.peek());
  registers_->pc_.poke(addr);
}

void Instructions::se_3xkk(regnb_t vx, uint8_t byte) {
  if (registers_->v_[vx].peek() == byte) { registers_->pc_.increment(2); }
}

void Instructions::sne_4xkk(regnb_t vx, uint8_t byte) {
  if (registers_->v_[vx].peek() != byte) { registers_->pc_.increment(2); }
}

void Instructions::se_5xy0(regnb_t vx, regnb_t vy) {
  if (registers_->v_[vx].peek() == registers_->v_[vy].peek()) { registers_->pc_.increment(2); }
}

void Instructions::ld_6xkk(regnb_t vx, uint8_t byte) { registers_->v_[vx].poke(byte); }

void Instructions::add_7xkk(regnb_t vx, uint8_t byte) {
  registers_->v_[vx].poke(registers_->v_[vx].peek() + byte);
}

void Instructions::ld_8xy0(regnb_t vx, regnb_t vy) {
  registers_->v_[vx].poke(registers_->v_[vy].peek());
}

void Instructions::or_8xy1(regnb_t vx, regnb_t vy) {
  registers_->v_[vx].poke(registers_->v_[vx].peek() | registers_->v_[vy].peek());
}

void Instructions::and_8xy2(regnb_t vx, regnb_t vy) {
  registers_->v_[vx].poke(registers_->v_[vx].peek() & registers_->v_[vy].peek());
}

void Instructions::xor_8xy3(regnb_t vx, regnb_t vy) {
  registers_->v_[vx].poke(registers_->v_[vx].peek() ^ registers_->v_[vy].peek());
}

void Instructions::add_8xy4(regnb_t vx, regnb_t vy) {
  registers_->v_[0xf].poke((registers_->v_[vx].peek() + registers_->v_[vy].peek()) > 0xff);
  registers_->v_[vx].poke(registers_->v_[vx].peek() + registers_->v_[vy].peek());
}

void Instructions::sub_8xy5(regnb_t vx, regnb_t vy) {
  registers_->v_[0xf].poke(registers_->v_[vx].peek() > registers_->v_[vy].peek());
  registers_->v_[vx].poke(registers_->v_[vx].peek() - registers_->v_[vy].peek());
}

/**
 * rmk Ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy6-and-8xye-shift
 * @param vx
 * @param vy
 */
void Instructions::shr_8xy6(regnb_t vx, regnb_t vy) {
  //  registers_->v_[vx].poke(registers_->v_[vy].peek());// Should be disabled at will by conf
  registers_->v_[0xf].poke(registers_->v_[vx].peek() & 0x1);
  registers_->v_[vx].poke(registers_->v_[vx].peek() >> 1);
}

void Instructions::subn_8xy7(regnb_t vx, regnb_t vy) {
  registers_->v_[0xf].poke(registers_->v_[vy].peek() > registers_->v_[vx].peek());
  registers_->v_[vx].poke(registers_->v_[vy].peek() - registers_->v_[vx].peek());
}

/**
 * rmk Ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy6-and-8xye-shift
 * @param vx
 * @param vy
 */
void Instructions::shl_8xyE(regnb_t vx, regnb_t vy) {
  //  registers_->v_[vx].poke(registers_->v_[vy].peek());// Should be disabled at will by conf
  registers_->v_[0xf].poke(registers_->v_[vx].peek() >> 7);
  registers_->v_[vx].poke(registers_->v_[vx].peek() << 1);
}

void Instructions::sne_9xy0(regnb_t vx, regnb_t vy) {
  if (registers_->v_[vx].peek() != registers_->v_[vy].peek()) { registers_->pc_.increment(2); }
}

/**
 * rmk Ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#annn-set-index
 * @param addr
 */
void Instructions::ld_Annn(address_t addr) { registers_->i_.poke(addr); }

/**
 * rmk Ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#bnnn-jump-with-offset
 * @param addr
 */
void Instructions::jp_Bnnn(address_t addr) {
  registers_->pc_.poke(addr + registers_->v_[0].peek());
}

void Instructions::rnd_Cxkk(regnb_t vx, uint8_t byte) {
  registers_->v_[vx].poke(randbyte->operator()(mt) & byte);
}

/**
 * n is the height of the sprite.
 * sprites are 8 pixels (8 bytes) wide.
 * @param vx
 * @param vy
 * @param n
 */
void Instructions::drw_Dxyn(regnb_t vx, regnb_t vy, uint8_t n) {
  y_ = registers_->v_[vy].peek() % 32;
  registers_->v_[0xf].poke(0);

  for (int row = 0; row < n; row++) {
    sprite_byte_ = memory_->peek(registers_->i_.peek() + row);
    x_ = registers_->v_[vx].peek() % 64;

    for (int pixel = 0; pixel < 8; pixel++) {
      sprite_pixel_is_on_ = sprite_byte_ & (uint8_t) pow(2, 7 - pixel);
      disp_pixel_is_on_ = interface_->is_pixel_on(x_, y_);

      if (sprite_pixel_is_on_ && disp_pixel_is_on_) {
        interface_->draw_pixel(x_, y_, false);
        registers_->v_[0xf].poke(1);
      } else if (sprite_pixel_is_on_) {
        interface_->draw_pixel(x_, y_, true);
      }

      x_++;
    }
    y_++;
  }
  interface_->render();
}

void Instructions::skp_Ex9E(regnb_t vx) {
  if (interface_->is_pressed(registers_->v_[vx].peek())) { registers_->pc_.increment(2); }
}

void Instructions::sknp_ExA1(regnb_t vx) {
  if (!interface_->is_pressed(registers_->v_[vx].peek())) { registers_->pc_.increment(2); }
}

void Instructions::ld_Fx07(regnb_t vx) { registers_->v_[vx].poke(registers_->dt_.peek()); }

void Instructions::ld_Fx0A(regnb_t vx) {
  if (interface_->get_any_pressed() != 0x10) {
    registers_->v_[vx].poke(interface_->get_any_pressed());
    return;
  }
  registers_->pc_.decrement(2);
}

void Instructions::ld_Fx15(regnb_t vx) { registers_->dt_.poke(registers_->v_[vx].peek()); }

void Instructions::ld_Fx18(regnb_t vx) { registers_->st_.poke(registers_->v_[vx].peek()); }

/**
 * @param vx
 */
void Instructions::add_Fx1E(regnb_t vx) {
  registers_->v_[0xf].poke((registers_->i_.peek() + registers_->v_[vx].peek()) > 0xFFF);

  registers_->i_.poke(registers_->i_.peek() + registers_->v_[vx].peek());
}

void Instructions::ld_Fx29(regnb_t vx) {
  registers_->i_.poke((registers_->v_[vx].peek() & 0xf) * 5);
}

void Instructions::ld_Fx33(regnb_t vx) {
  memory_->poke(registers_->v_[vx].peek() / 100 % 10, registers_->i_.peek());
  memory_->poke(registers_->v_[vx].peek() / 10 % 10, registers_->i_.peek() + 1);
  memory_->poke(registers_->v_[vx].peek() % 10, registers_->i_.peek() + 2);
}

/**
 * rmk ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#fx55-and-fx65-store-and-load-memory
 * @param vx
 */
void Instructions::ld_Fx55(regnb_t vx) {
  for (int i = 0; i <= vx; i++) {
    memory_->poke(registers_->v_[i].peek(), registers_->i_.peek() + i);
    //    registers_->i_.increment();
  }
}

/**
 * rmk ambiguous https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#fx55-and-fx65-store-and-load-memory
 * @param vx
 */
void Instructions::ld_Fx65(regnb_t vx) {
  for (int i = 0; i <= vx; i++) {
    registers_->v_[i].poke(memory_->peek(registers_->i_.peek() + i));
    //    registers_->i_.increment();
  }
}
