// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include <iostream>
#include <memory>

#include "Configuration.h"
#include "Interpreter.h"
#include "tclap/CmdLine.h"

int main(int argc, char ** argv) {
  // Argument parsing with TCLAP
  try {
    TCLAP::CmdLine cmd("CHIP8 Interpreter written in C++, "
                       "MIT License Copyright (c) 2021 Maxandre Ogeret, "
                       "https://github.com/MaxandreOgeret/chip8_interpreter",
                       ' ', "0.1");
    TCLAP::SwitchArg conf_4_arg("4", "4", "OR/AND/XOR 8xy1/8xy2/8xy3 will reset vf to 0.", cmd,
                                false);
    TCLAP::SwitchArg conf_3_arg("3", "3", "Store/Load Fx55/Fx65 will increment i.", cmd, false);
    TCLAP::SwitchArg conf_2_arg("2", "2",
                                "Jump with offset Bnnn becomes Bxnn and add x offset to the jump.",
                                cmd, false);
    TCLAP::SwitchArg conf_1_arg(
            "1", "1", "Shift instructions 8xy6 and 8xyE will also set xv to the value of xy.", cmd,
            false);
    TCLAP::ValueArg<int> freq_arg("f", "Frequency", "CPU Frequency (Default: 500Hz)", false, 500,
                                  "value");
    cmd.add(freq_arg);
    TCLAP::UnlabeledValueArg<std::string> rom_path_arg("rom_path", "Path to CHIP8 rom.", true, "",
                                                       "Path");
    cmd.add(rom_path_arg);
    cmd.parse(argc, argv);

    std::unique_ptr<Configuration> configuration = std::make_unique<Configuration>(
            rom_path_arg.getValue(), freq_arg.getValue(), conf_1_arg.getValue(),
            conf_2_arg.getValue(), conf_3_arg.getValue(), conf_4_arg.getValue());

    std::unique_ptr<Interpreter> interpreter = std::make_unique<Interpreter>(*configuration);
    interpreter->loop();
  } catch (TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }
  return 0;
}