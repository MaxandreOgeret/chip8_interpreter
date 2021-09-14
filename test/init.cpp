// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Memory.h"
#include "gtest/gtest.h"
#include <Instructions.h>
#include <Interface.h>
#include <RomParser.h>
#include <memory>
#include <register/RegisterManager.h>

const unsigned short int FREQ = 500;

TEST(init, init_memory) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();

  for (mem::address_t i=memory->FONT_.size(); i<4096; i++) {
    EXPECT_EQ(memory->peek(i), 0x0);
  }

  for (mem::address_t i=0; i<memory->FONT_.size(); i++) {
    EXPECT_EQ(memory->peek(i), memory->FONT_[i]);
  }

}

TEST(init, init_registers) {
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(1);

  // V0 -> VF
  for (reg::regnb_t i=0; i<16; i++) {
    EXPECT_EQ(registers->v_[i].peek(), 0x0);
  }

  EXPECT_EQ(registers->pc_.peek(), 0x200);
  EXPECT_EQ(registers->i_.peek(), 0x0);
  EXPECT_EQ(registers->st_.peek(), 0x0);
  EXPECT_EQ(registers->dt_.peek(), 0x0);
  EXPECT_EQ(registers->stack_.size(), 0x0);
}

TEST(init, init_display) {
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> display = std::make_shared<Interface>(registers);

  for (int x=0; x<display->SIZE_X_; x++) {
    for (int y=0; y<display->SIZE_Y_; y++) {
      EXPECT_FALSE(display->is_pixel_on(x, y));
    }
  }
}

TEST(init, init_parser) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> display = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, display);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(memory->peek(0x200), 0x00);
  EXPECT_EQ(memory->peek(0x201), 0xe0);
}

TEST(init, init_parser_fail) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> display = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, display);
  EXPECT_THROW(std::make_shared<RomParser>("", memory, registers, instructions), std::runtime_error);
}