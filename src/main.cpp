// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include <iostream>
#include <memory>

#include "Emulator.h"
#include "tclap/CmdLine.h"

int main(int argc, char ** argv) {
  // Argument parsing with TCLAP
  try {
    TCLAP::CmdLine cmd("CHIP8 Interpreter written in C++", ' ', "0.1");
    TCLAP::SwitchArg conf_4_arg("4", "4", "OR/AND/XOR 8xy1/8xy2/8xy3 will reset vf to 0.", cmd,
                                false);
    TCLAP::SwitchArg conf_3_arg("3", "3", "Store/Load Fx55/Fx65 will increment i.", cmd, false);
    TCLAP::SwitchArg conf_2_arg("2", "2",
                                "Jump with offset Bnnn becomes Bxnn and add x offset to the jump.",
                                cmd, false);
    TCLAP::SwitchArg conf_1_arg("1", "1", "Shift instructions 8xy6 and 8xyE will ignore vy.", cmd,
                                false);
    TCLAP::UnlabeledValueArg<std::string> rom_path_arg("rom_path", "Path to CHIP8 rom.", true, "",
                                                       "Path");
    cmd.add(rom_path_arg);
    cmd.parse(argc, argv);
    bool conf_1 = conf_1_arg.getValue();
    bool conf_2 = conf_2_arg.getValue();
    bool conf_3 = conf_3_arg.getValue();
    bool conf_4 = conf_4_arg.getValue();
    std::string rom_path = rom_path_arg.getValue();
    std::unique_ptr<Emulator> emulator = std::make_unique<Emulator>(rom_path);
    emulator->loop();
  } catch (TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }
  return 0;
}
