// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Configuration.h"


Configuration::Configuration(const std::string & romPath, bool c8Xy68XyESetsVy,
                             bool cBnnnBecomesBxnn, bool cFx55Fx65IncrementsI,
                             bool c8Xy18Xy28Xy3ResetVf)
    : rom_path_(romPath), c_8xy6_8xyE_sets_vy_(c8Xy68XyESetsVy),
      c_Bnnn_becomes_Bxnn_(cBnnnBecomesBxnn), c_Fx55_Fx65_increments_i_(cFx55Fx65IncrementsI),
      c_8xy1_8xy2_8xy3_reset_vf_(c8Xy18Xy28Xy3ResetVf) {}

const std::string & Configuration::getRomPath() const {
  return rom_path_;
}

bool Configuration::isC8Xy68XyESetsVy() const {
  return c_8xy6_8xyE_sets_vy_;
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
