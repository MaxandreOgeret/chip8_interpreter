// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "register/RegisterManager.h"

using namespace reg;

RegisterManager::RegisterManager(const unsigned short int frequency) {
  v_ = std::vector<Register<uint8_t>>(0x10);
  pc_.poke(0x200);
  decrement_interval_ = frequency / 60;
}

void RegisterManager::trigger_timers() {
  counter_++;
  if (counter_ == decrement_interval_) {
    counter_ = 0;

    if (dt_.peek() > 0) { dt_.decrement(1); }

    if (st_.peek() > 0) { st_.decrement(1); }
  }
}
