// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "RomParser.h"

RomParser::RomParser(Configuration & configuration, std::shared_ptr<mem::Memory> memory,
                     std::shared_ptr<reg::RegisterManager> registerManager,
                     std::shared_ptr<Instructions> instructions)
    : configuration_(configuration), memory_(memory), registers_(registerManager),
      instructions_(instructions) {
  std::cout << "Loading ROM: " << configuration_.getRomPath() << "\n";

  source_ = std::ifstream(configuration_.getRomPath(), std::ios_base::binary);

  if (!source_) { throw std::runtime_error("Unable to open rom."); }

  contents_ = std::vector<uint8_t>((std::istreambuf_iterator<char>(source_)),
                                   std::istreambuf_iterator<char>());

  memory_->poke(contents_, 0x200);
  contents_.clear();
  source_.close();
}

void RomParser::step() {
  opcode_ = (memory_->peek(registers_->pc_.peek()) << 8) +
            (memory_->peek(registers_->pc_.peek() + 1));
  registers_->pc_.increment(2);
}

void RomParser::decode() {
  switch (get_from_opcode(opcode_, 0xF000)) {
    case 0x0:
      switch (get_from_opcode(opcode_, 0x000F)) {
        case 0x0:
          instructions_->cls_00E0();
          return;
        case 0xE:
          instructions_->ret_00EE();
          return;
        default:
          instructions_->sys_0nnn(get_from_opcode(opcode_, 0x0FFF));
          return;
      }
      break;
    case 0x1:
      instructions_->jp_1nnn(get_from_opcode(opcode_, 0x0FFF));
      return;
    case 0x2:
      instructions_->call_2nnn(get_from_opcode(opcode_, 0x0FFF));
      return;
    case 0x3:
      instructions_->se_3xkk(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00FF));
      return;
    case 0x4:
      instructions_->sne_4xkk(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00FF));
      return;
    case 0x5:
      instructions_->se_5xy0(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00F0));
      return;
    case 0x6:
      instructions_->ld_6xkk(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00FF));
      return;
    case 0x7:
      instructions_->add_7xkk(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00FF));
      return;
    case 0x8:
      switch (get_from_opcode(opcode_, 0x000F)) {
        case 0x0:
          instructions_->ld_8xy0(get_from_opcode(opcode_, 0x0F00),
                                 get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x1:
          instructions_->or_8xy1(get_from_opcode(opcode_, 0x0F00),
                                 get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x2:
          instructions_->and_8xy2(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x3:
          instructions_->xor_8xy3(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x4:
          instructions_->add_8xy4(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x5:
          instructions_->sub_8xy5(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x6:
          instructions_->shr_8xy6(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
        case 0x7:
          instructions_->subn_8xy7(get_from_opcode(opcode_, 0x0F00),
                                   get_from_opcode(opcode_, 0x00F0));
          return;
        case 0xE:
          instructions_->shl_8xyE(get_from_opcode(opcode_, 0x0F00),
                                  get_from_opcode(opcode_, 0x00F0));
          return;
      }
      break;
    case 0x9:
      instructions_->sne_9xy0(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00F0));
      return;
    case 0xA:
      instructions_->ld_Annn(get_from_opcode(opcode_, 0x0FFF));
      return;
    case 0xB:
      if (configuration_.isCBnnnBecomesBxnn()) {
        instructions_->jp_Bxnn(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x0FFF));
      } else {
        instructions_->jp_Bnnn(get_from_opcode(opcode_, 0x0FFF));
      }
      return;
    case 0xC:
      instructions_->rnd_Cxkk(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00FF));
      return;
    case 0xD:
      instructions_->drw_Dxyn(get_from_opcode(opcode_, 0x0F00), get_from_opcode(opcode_, 0x00F0),
                              get_from_opcode(opcode_, 0x000F));
      return;
    case 0xE:
      switch (get_from_opcode(opcode_, 0x00FF)) {
        case 0x9E:
          instructions_->skp_Ex9E(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0xA1:
          instructions_->sknp_ExA1(get_from_opcode(opcode_, 0x0F00));
          return;
      }
      break;
    case 0xF:
      switch (get_from_opcode(opcode_, 0x00FF)) {
        case 0x07:
          instructions_->ld_Fx07(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x0A:
          instructions_->ld_Fx0A(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x15:
          instructions_->ld_Fx15(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x18:
          instructions_->ld_Fx18(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x1E:
          instructions_->add_Fx1E(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x29:
          instructions_->ld_Fx29(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x33:
          instructions_->ld_Fx33(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x55:
          instructions_->ld_Fx55(get_from_opcode(opcode_, 0x0F00));
          return;
        case 0x65:
          instructions_->ld_Fx65(get_from_opcode(opcode_, 0x0F00));
          return;
      }
      break;
  }

  ss << "0x" << std::hex << opcode_;
  throw std::runtime_error("UNKNOW OPCODE: " + ss.str());
}

uint16_t RomParser::get_from_opcode(const uint16_t & opcode, const uint16_t mask) {
  if (!(mask == 0x0FFF || mask == 0x00FF || mask == 0xF000 || mask == 0x0F00 || mask == 0x00F0 ||
        mask == 0x000F)) {
    throw std::runtime_error("Opcode mask is invalid: " + std::to_string(mask));
  }

  uint16_t v = mask;

  // c counts v's trailing zero bits,
  // so if v is 1101000 (base 2), then c will be 3
  int c;

  // Set v's trailing 0s to 1s and zero rest
  v = (v ^ (v - 1)) >> 1;
  for (c = 0; v; c++) { v >>= 1; }

  return (opcode & mask) >> c;
}

void RomParser::set_opcode(uint16_t opcode) {
  opcode_ = opcode;
}
