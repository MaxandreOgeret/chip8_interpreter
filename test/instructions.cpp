// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Memory.h"
#include "gtest/gtest.h"
#include <Instructions.h>
#include <Interface.h>
#include <RomParser.h>
#include <SDL_keycode.h>
#include <memory>
#include <register/RegisterManager.h>

const unsigned short int FREQ = 500;

TEST(instructions, get_from_opcode) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  // INVALID MASKS
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0xFFFF), std::runtime_error);
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0xF00F), std::runtime_error);
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0x0000), std::runtime_error);
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0xFF00), std::runtime_error);
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0x0F0F), std::runtime_error);
  EXPECT_THROW(romParser->get_from_opcode(0x6789, 0xF0F0), std::runtime_error);

  // VALID MASKS
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0x0FFF), 0x789);
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0x00FF), 0x89);
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0xF000), 0x6);
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0x0F00), 0x7);
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0x00F0), 0x8);
  EXPECT_EQ(romParser->get_from_opcode(0x6789, 0x000F), 0x9);

  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0x0FFF), 0xFFF);
  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0x00FF), 0xFF);
  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0xF000), 0xF);
  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0x0F00), 0xF);
  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0x00F0), 0xF);
  EXPECT_EQ(romParser->get_from_opcode(0xFFFF, 0x000F), 0xF);

  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0x0FFF), 0x0);
  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0x00FF), 0x0);
  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0xF000), 0x0);
  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0x0F00), 0x0);
  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0x00F0), 0x0);
  EXPECT_EQ(romParser->get_from_opcode(0x0000, 0x000F), 0x0);
}

TEST(instructions, 00E0) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  for (int x=0; x<interface->SIZE_X_; x++) {
    for (int y=0; y<interface->SIZE_Y_; y++) {
      interface->draw_pixel(x, y, true);
    }
  }

  for (int x=0; x<interface->SIZE_X_; x++) {
    for (int y=0; y<interface->SIZE_Y_; y++) {
      EXPECT_TRUE(interface->is_pixel_on(x, y));
    }
  }

  romParser->set_opcode(0x00E0);
  romParser->decode();

  for (int x=0; x<interface->SIZE_X_; x++) {
    for (int y=0; y<interface->SIZE_Y_; y++) {
      EXPECT_FALSE(interface->is_pixel_on(x, y));
    }
  }
}

TEST(instructions, 00EE) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->stack_.size(), 0);
  EXPECT_EQ(registers->sp_.peek(), 0);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  registers->stack_.push_back(0x0FFF);
  registers->sp_.increment(1);

  EXPECT_EQ(registers->stack_.size(), 1);
  EXPECT_EQ(registers->sp_.peek(), 1);

  romParser->set_opcode(0x00EE);
  romParser->decode();

  EXPECT_EQ(registers->pc_.peek(), 0x0FFF);
  EXPECT_EQ(registers->stack_.size(), 0);
  EXPECT_EQ(registers->sp_.peek(), 0);
}

TEST(instructions, 1nnn) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->pc_.peek(), 0x0200);

  romParser->set_opcode(0x1DEF);
  romParser->decode();

  EXPECT_EQ(registers->pc_.peek(), 0x0DEF);
}

TEST(instructions, 2nnn) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->stack_.size(), 0);
  EXPECT_EQ(registers->sp_.peek(), 0);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0x2DEF);
  romParser->decode();

  EXPECT_EQ(registers->sp_.peek(), 1);
  EXPECT_EQ(registers->stack_.size(), 1);
  EXPECT_EQ(registers->stack_.back(), 0x200);
  EXPECT_EQ(registers->pc_.peek(), 0xDEF);
}


TEST(instructions, 3xkk) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x4].peek(), 0);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  registers->v_[0x4].poke(0xFE);

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0x34FE);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x202);

  romParser->set_opcode(0x34FF);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x202);
}

TEST(instructions, 4xkk) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x4].peek(), 0);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  registers->v_[0x4].poke(0xFE);

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0x34FF);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0x34FE);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x202);

}

TEST(instructions, 5xy0) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x4].peek(), 0);
  EXPECT_EQ(registers->v_[0xf].peek(), 0);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  registers->v_[0x4].poke(0xFE);
  registers->v_[0xf].poke(0xFF);

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->v_[0xf].peek(), 0xFF);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  registers->v_[0x4].poke(0xFE);
  registers->v_[0xf].poke(0xFE);

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->v_[0xf].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0x54f0);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->v_[0xf].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x202);

  romParser->set_opcode(0x5f40);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x4].peek(), 0xFE);
  EXPECT_EQ(registers->v_[0xf].peek(), 0xFE);
  EXPECT_EQ(registers->pc_.peek(), 0x204);
}

TEST(instructions, 6xkk) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0);
  EXPECT_EQ(registers->v_[0xF].peek(), 0);

  romParser->set_opcode(0x60EE);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0xEE);

  romParser->set_opcode(0x6FEE);
  romParser->decode();

  EXPECT_EQ(registers->v_[0xF].peek(), 0xEE);
}

TEST(instructions, 7xkk) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0);
  EXPECT_EQ(registers->v_[0xF].peek(), 0);

  romParser->set_opcode(0x70FF);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0xFF);
  EXPECT_EQ(registers->v_[0xF].peek(), 0);

  romParser->set_opcode(0x7001);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xF].peek(), 0);
}

TEST(instructions, 8xy0) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  romParser->set_opcode(0x80E0);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  registers->v_[0xE].poke(0x0C);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x0C);

  romParser->set_opcode(0x80E0);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x0C);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x0C);

}

TEST(instructions, 8xy1) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  romParser->set_opcode(0x80E1);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  registers->v_[0x0].poke(0b01001001);
  registers->v_[0xE].poke(0b10100001);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01001001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);

  romParser->set_opcode(0x80E1);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b11101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);
}

TEST(instructions, 8xy2) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  romParser->set_opcode(0x80E2);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  registers->v_[0x0].poke(0b01101001);
  registers->v_[0xE].poke(0b10100001);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);

  romParser->set_opcode(0x80E2);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b00100001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);
}

TEST(instructions, 8xy3) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  romParser->set_opcode(0x80E3);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  registers->v_[0x0].poke(0b01101001);
  registers->v_[0xE].poke(0b10100001);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);

  romParser->set_opcode(0x80E3);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b11001000);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);
}

TEST(instructions, 8xy4) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  romParser->set_opcode(0x80E4);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x00);

  //

  registers->v_[0x0].poke(0b01101001);
  registers->v_[0xE].poke(0b00100001);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b00100001);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);

  romParser->set_opcode(0x80E4);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b10001010);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b00100001);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);

  //

  registers->v_[0x0].poke(0b01101001);
  registers->v_[0xE].poke(0b10100001);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);

  romParser->set_opcode(0x80E4);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b00001010);
  EXPECT_EQ(registers->v_[0xE].peek(), 0b10100001);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);

  //

  registers->v_[0x0].poke(0b01101001);
  registers->v_[0xE].poke(0x0);

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x0);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);

  romParser->set_opcode(0x80E4);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0b01101001);
  EXPECT_EQ(registers->v_[0xE].peek(), 0x0);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);
}


TEST(instructions, 8xy5) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x1].poke(0x01);
  registers->v_[0x2].poke(0x0f);

  romParser->set_opcode(0x8215);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x1].peek(), 0x01);
  EXPECT_EQ(registers->v_[0x2].peek(), 0x0E);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);

  //

  registers->v_[0x1].poke(0x0f);
  registers->v_[0x2].poke(0x01);

  romParser->set_opcode(0x8215);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x1].peek(), 0x0f);
  EXPECT_EQ(registers->v_[0x2].peek(), 0xf2);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);
}

TEST(instructions, 8xy6) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x00);
  registers->v_[0x1].poke(0xff);

  romParser->set_opcode(0x8016);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x7f);
  EXPECT_EQ(registers->v_[0x1].peek(), 0xff);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);
//
  registers->v_[0x0].poke(0x00);
  registers->v_[0x1].poke(0xfe);

  romParser->set_opcode(0x8016);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x7f);
  EXPECT_EQ(registers->v_[0x1].peek(), 0xfe);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);

}

TEST(instructions, 8xy7) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x00);
  registers->v_[0x1].poke(0x7f);

  romParser->set_opcode(0x8017);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x7f);
  EXPECT_EQ(registers->v_[0x1].peek(), 0x7f);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);

  //

  registers->v_[0x0].poke(0x7f);
  registers->v_[0x1].poke(0x00);

  romParser->set_opcode(0x8017);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x81);
  EXPECT_EQ(registers->v_[0x1].peek(), 0x00);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);
}

TEST(instructions, 8xye) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x00);
  registers->v_[0x1].poke(0xff);

  romParser->set_opcode(0x801e);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0xFE);
  EXPECT_EQ(registers->v_[0x1].peek(), 0xff);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x01);
//
  registers->v_[0x0].poke(0x00);
  registers->v_[0x1].poke(0x7f);

  romParser->set_opcode(0x801e);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0xfe);
  EXPECT_EQ(registers->v_[0x1].peek(), 0x7f);
  EXPECT_EQ(registers->v_[0xF].peek(), 0x00);
}

TEST(instructions, 9xy0) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x20);
  registers->v_[0x1].poke(0x10);

  romParser->set_opcode(0x9010);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x20);
  EXPECT_EQ(registers->v_[0x1].peek(), 0x10);
  EXPECT_EQ(registers->pc_.peek(), 0x202);

  registers->v_[0x0].poke(0x20);
  registers->v_[0x1].poke(0x20);

  romParser->set_opcode(0x9010);
  romParser->decode();

  EXPECT_EQ(registers->v_[0x0].peek(), 0x20);
  EXPECT_EQ(registers->v_[0x1].peek(), 0x20);
  EXPECT_EQ(registers->pc_.peek(), 0x202);
}

TEST(instructions, Annn) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  romParser->set_opcode(0xA345);
  romParser->decode();

  EXPECT_EQ(registers->i_.peek(), 0x345);
}

TEST(instructions, Bnnn) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x1);

  romParser->set_opcode(0xB345);
  romParser->decode();

  EXPECT_EQ(registers->pc_.peek(), 0x346);
}

/**
 * This doesnt test the random number. Only the mask.
 */
TEST(instructions, Cxkk) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  romParser->set_opcode(0xC055);
  romParser->decode();

  EXPECT_EQ(registers->v_[0].peek() & 0xAA, 0x0);
}

TEST(instructions, Dxyn) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  // one sprite on top left (with modulo)
  registers->v_[1].poke(64);
  registers->v_[2].poke(32);

  romParser->set_opcode(0xF029);
  romParser->decode();
  romParser->set_opcode(0xd125);
  romParser->decode();

  std::vector<bool> row1 = {true, true, true, true, false, false, false, false};
  std::vector<bool> row2 = {true, false, false, true, false, false, false, false};

  for (int i=0; i<8; i++) {
    EXPECT_EQ(interface->is_pixel_on(i, 0), row1[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 1), row2[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 2), row2[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 3), row2[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 4), row1[i]);
  }
  EXPECT_EQ(registers->v_[0xf].peek(), 0x0);

  // draw same sprite (should remove it)
  registers->v_[1].poke(0);
  registers->v_[2].poke(0);

  romParser->set_opcode(0xF029);
  romParser->decode();
  romParser->set_opcode(0xd005);
  romParser->decode();

  for (int x=0; x<interface->SIZE_X_; x++) {
    for (int y=0; y<interface->SIZE_Y_; y++) {
      EXPECT_FALSE(interface->is_pixel_on(x, y));
    }
  }
  EXPECT_EQ(registers->v_[0xf].peek(), 0x1);

  // sprite overflowing on x and y
  registers->v_[1].poke(62);
  registers->v_[2].poke(30);

  romParser->set_opcode(0xF029);
  romParser->decode();
  romParser->set_opcode(0xd125);
  romParser->decode();

  std::vector<bool> row3 = {
          false, true, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, true, false,
  };

  std::vector<bool> row4 = {
          true, true, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, false, false,
          false, false, false, false, false, false, true, true,
  };

  EXPECT_EQ(registers->v_[0xf].peek(), 0x0);
  for (int i=0; i<64; i++) {
    EXPECT_EQ(interface->is_pixel_on(i, 0), row3[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 1), row3[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 2), row4[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 62), row4[i]);
    EXPECT_EQ(interface->is_pixel_on(i, 63), row3[i]);
  }

  for (int x=2; x<interface->SIZE_X_-2; x++) {
    for (int y=0; y<interface->SIZE_Y_; y++) {
      EXPECT_FALSE(interface->is_pixel_on(x, y));
    }
  }
}

TEST(instructions, Ex9E) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0].poke(0x0);
  registers->v_[1].poke(0x1);

  interface->simulate_keypress(SDLK_1);

  for (int i=0; i<=100; i++) {
    interface->poll_events();
    interface->get_keys();
    if (interface->is_pressed(0x1)) {
      break;
    } else if (i == 100) {
      throw std::runtime_error("Unable to detect keypress after 100 tries.");
    }
  }
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0xE09E);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0xE19E);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x202);
}

TEST(instructions, ExA1) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0].poke(0x0);
  registers->v_[1].poke(0x1);

  interface->simulate_keypress(SDLK_1);

  for (int i=0; i<=100; i++) {
    interface->poll_events();
    interface->get_keys();
    if (interface->is_pressed(0x1)) {
      break;
    } else if (i == 100) {
      throw std::runtime_error("Unable to detect keypress after 100 tries.");
    }
  }
  EXPECT_EQ(registers->pc_.peek(), 0x200);

  romParser->set_opcode(0xE0A1);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x202);

  romParser->set_opcode(0xE1A1);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x202);

}

TEST(instructions, Fx07) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->dt_.poke(0x0);
  romParser->set_opcode(0xF007);
  romParser->decode();
  EXPECT_EQ(registers->v_[0x0].peek(), 0x0);

  registers->dt_.poke(0xf);
  romParser->set_opcode(0xFF07);
  romParser->decode();
  EXPECT_EQ(registers->v_[0xf].peek(), 0xf);

}

TEST(instructions, Fx15) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0xf].poke(0xf);
  romParser->set_opcode(0xFF15);
  romParser->decode();
  EXPECT_EQ(registers->dt_.peek(), 0xf);

  registers->v_[0xf].poke(0x0);
  romParser->set_opcode(0xFF15);
  romParser->decode();
  EXPECT_EQ(registers->dt_.peek(), 0x0);
}

TEST(instructions, Fx18) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0xf].poke(0xf);
  romParser->set_opcode(0xFF18);
  romParser->decode();
  EXPECT_EQ(registers->st_.peek(), 0xf);

  registers->v_[0xf].poke(0x0);
  romParser->set_opcode(0xFF18);
  romParser->decode();
  EXPECT_EQ(registers->st_.peek(), 0x0);
}

TEST(instructions, Fx1E) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->i_.poke(0x0);
  registers->v_[0xe].poke(0x0);

  romParser->set_opcode(0xFE1E);
  romParser->decode();
  EXPECT_EQ(registers->i_.peek(), 0x0);


  registers->i_.poke(0x3);
  registers->v_[0xe].poke(0x5);

  romParser->set_opcode(0xFE1E);
  romParser->decode();
  EXPECT_EQ(registers->i_.peek(), 0x8);
  EXPECT_EQ(registers->v_[0xf].peek(), 0x0);


  registers->i_.poke(0x0ffe);
  registers->v_[0xe].poke(0x2);

  romParser->set_opcode(0xFE1E);
  romParser->decode();
  EXPECT_EQ(registers->i_.peek(), 0x1000);
  EXPECT_EQ(registers->v_[0xf].peek(), 0x1);
}

TEST(instructions, Fx0A) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  EXPECT_EQ(registers->pc_.peek(), 0x200);
  registers->v_[0xD].poke(0xa);
  romParser->set_opcode(0xFD0A);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x1fe);
  EXPECT_EQ(registers->v_[0xD].peek(), 0xa);


  interface->simulate_keypress(SDLK_1);

  for (int i=0; i<=100; i++) {
    interface->poll_events();
    interface->get_keys();
    if (interface->is_pressed(0x1)) {
      break;
    } else if (i == 100) {
      throw std::runtime_error("Unable to detect keypress after 100 tries.");
    }
  }

  registers->v_[0xD].poke(0xa);
  romParser->set_opcode(0xFD0A);
  romParser->decode();
  EXPECT_EQ(registers->pc_.peek(), 0x1fe);
  EXPECT_EQ(registers->v_[0xD].peek(), 0x1);


}

TEST(instructions, Fx29) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x0000);
  romParser->set_opcode(0xF029);
  romParser->decode();
  EXPECT_EQ(registers->i_.peek(), 0);

  registers->v_[0x0].poke(0xF0);
  romParser->set_opcode(0xF029);
  romParser->decode();
  EXPECT_EQ(registers->i_.peek(), 0);

  for (uint8_t i=0; i<=0xf; i++) {
    registers->v_[i].poke(i);
    instructions->ld_Fx29(i);
    EXPECT_EQ(registers->i_.peek(), i*5);
  }
}

TEST(instructions, Fx33) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  registers->v_[0x0].poke(0x0000);
  romParser->set_opcode(0xF033);
  romParser->decode();
  EXPECT_EQ(memory->peek(registers->i_.peek()), 0);
  EXPECT_EQ(memory->peek(registers->i_.peek()+1), 0);
  EXPECT_EQ(memory->peek(registers->i_.peek()+2), 0);

  registers->i_.poke(3);
  registers->v_[0x0].poke(255);
  romParser->set_opcode(0xF033);
  romParser->decode();
  EXPECT_EQ(memory->peek(registers->i_.peek()), 2);
  EXPECT_EQ(memory->peek(registers->i_.peek()+1), 5);
  EXPECT_EQ(memory->peek(registers->i_.peek()+2), 5);

  for (uint8_t i=0; i<=0x33; i++) {
    registers->v_[0x0].poke(i*5);
    romParser->set_opcode(0xF033);
    romParser->decode();

    EXPECT_EQ(
            memory->peek(registers->i_.peek()) * 100 +
            memory->peek(registers->i_.peek()+1) * 10 +
            memory->peek(registers->i_.peek()+2)
                    , i*5);
    std::cout << (int)i*5 << "\n";
  }
}

TEST(instructions, Fx55) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  for (uint8_t i=0; i<=0xF; i++) {
    registers->v_[i].poke(i);
  }

  romParser->set_opcode(0xFF55);
  romParser->decode();

  for (uint8_t i=0; i<=0xF; i++) {
    EXPECT_EQ(memory->peek(registers->i_.peek()+i), registers->v_[i].peek());
  }
} // todo

TEST(instructions, Fx65) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  romParser->set_opcode(0xFF65);
  romParser->decode();

  for (uint8_t i=0; i<=0xF; i++) {
    EXPECT_EQ(memory->peek(registers->i_.peek()+i), registers->v_[i].peek());
  }
}

TEST(instructions, 00EA) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  romParser->set_opcode(0x800F);
  EXPECT_THROW(romParser->decode(), std::runtime_error);
}

TEST(instructions, 02D8) {
  std::shared_ptr<mem::Memory> memory = std::make_shared<mem::Memory>();
  std::shared_ptr<reg::RegisterManager> registers = std::make_shared<reg::RegisterManager>(FREQ);
  std::shared_ptr<Interface> interface = std::make_shared<Interface>(registers, true);
  std::shared_ptr<Instructions> instructions = std::make_shared<Instructions>(memory, registers, interface);
  std::shared_ptr<RomParser> romParser = std::make_shared<RomParser>("./test/cls.ch8", memory, registers, instructions);

  romParser->set_opcode(0x02D8);
  EXPECT_NO_THROW(romParser->decode());
}