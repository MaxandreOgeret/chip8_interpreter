// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Configuration.h"
Configuration::Configuration(bool c_8xy6_8xyE_ignore_vy, bool c_Bnnn_becomes_Bxnn,
                             bool c_Fx55_Fx65_increments_i, bool c_8xy1_8xy2_8xy3_reset_vf)
    : c_8xy6_8xyE_ignore_vy_(c_8xy6_8xyE_ignore_vy), c_Bnnn_becomes_Bxnn_(c_Bnnn_becomes_Bxnn),
      c_Fx55_Fx65_increments_i_(c_Fx55_Fx65_increments_i),
      c_8xy1_8xy2_8xy3_reset_vf_(c_8xy1_8xy2_8xy3_reset_vf) {}

bool Configuration::isC8Xy68XyEIgnoreVy() const {
  return c_8xy6_8xyE_ignore_vy_;
}

bool Configuration::isCBnnnBecomesBxnn() const {
  return c_Bnnn_becomes_Bxnn_;
}

bool Configuration::isCFx55Fx65IncrementsI() const {
  return c_Fx55_Fx65_increments_i_;
}

bool Configuration::isC8Xy18Xy28Xy3ResetVf() const {
  return c_8xy1_8xy2_8xy3_reset_vf_;
}
