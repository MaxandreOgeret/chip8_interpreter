// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Interpreter.h"

volatile static sig_atomic_t stop = 0;
const unsigned short int FREQ = 500;

using namespace std::chrono;

void inthand(int signum) {
  stop = 1;
}

Interpreter::Interpreter(std::shared_ptr<Configuration> configuration)
    : configuration_(configuration) {
  signal(SIGINT, inthand);
  signal(SIGTERM, inthand);

  memory_ = std::make_shared<mem::Memory>();
  registers_ = std::make_shared<reg::RegisterManager>(configuration->getFrequency());
  interface_ = std::make_shared<Interface>(registers_);
  instructions_ = std::make_shared<Instructions>(configuration, memory_, registers_, interface_);
  romParser_ = std::make_shared<RomParser>(configuration, memory_, registers_, instructions_);
}

void Interpreter::loop() {

  const milliseconds intervalPeriodMillis{
          static_cast<int>((1. / configuration_->getFrequency()) * 1000)};

  //Initialize the chrono timepoint & duration objects we'll be using over & over inside our sleep loop
  system_clock::time_point currentStartTime{system_clock::now()};
  system_clock::time_point nextStartTime{currentStartTime};

  while (!stop) {
    currentStartTime = system_clock::now();
    nextStartTime = currentStartTime + intervalPeriodMillis;

    registers_->trigger_timers();
    interface_->toogle_buzzer();
    interface_->poll_events();
    interface_->get_keys();
    romParser_->step();
    romParser_->decode();

    stop = stop || interface_->requests_close();
    std::this_thread::sleep_until(nextStartTime);
  }
}
