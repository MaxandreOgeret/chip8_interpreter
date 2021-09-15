// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_CONFIGURATION_H
#define CHIP8_CONFIGURATION_H


class Configuration {
public:
  Configuration(bool c_8xy6_8xyE_ignore_vy, bool c_Bnnn_becomes_Bxnn, bool c_Fx55_Fx65_increments_i,
                bool c_8xy1_8xy2_8xy3_reset_vf);

  bool isC8Xy68XyEIgnoreVy() const;
  bool isCBnnnBecomesBxnn() const;
  bool isCFx55Fx65IncrementsI() const;
  bool isC8Xy18Xy28Xy3ResetVf() const;

private:
  bool c_8xy6_8xyE_ignore_vy_;    // arg 1
  bool c_Bnnn_becomes_Bxnn_;      // arg 2
  bool c_Fx55_Fx65_increments_i_; // arg 3
  bool c_8xy1_8xy2_8xy3_reset_vf_;// arg 4
};


#endif//CHIP8_CONFIGURATION_H
