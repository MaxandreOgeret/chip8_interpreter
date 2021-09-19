// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_CONFIGURATION_H
#define CHIP8_CONFIGURATION_H

#include "string"

class Configuration {
public:
  Configuration(const std::string & romPath, int frequency, bool c8Xy68XyESetsVy,
                bool cBnnnBecomesBxnn, bool cFx55Fx65IncrementsI, bool c8Xy18Xy28Xy3ResetVf);


  /**
   * @returns rom path.
   */
  const std::string & getRomPath() const;

  /**
   * @returns state of config
   */
  bool isC8Xy68XyESetsVy() const;

  /**
   * @returns state of config
   */
  bool isCBnnnBecomesBxnn() const;

  /**
   * @returns state of config
   */
  bool isCFx55Fx65IncrementsI() const;

  /**
   * @returns state of config
   */
  bool isC8Xy18Xy28Xy3ResetVf() const;

  /**
   * @returns state of config
   */
  int getFrequency() const;

private:
  std::string rom_path_;
  int frequency_;
  bool c_8xy6_8xyE_sets_vy_;      // arg 1
  bool c_Bnnn_becomes_Bxnn_;      // arg 2
  bool c_Fx55_Fx65_increments_i_; // arg 3
  bool c_8xy1_8xy2_8xy3_reset_vf_;// arg 4
};


#endif//CHIP8_CONFIGURATION_H
