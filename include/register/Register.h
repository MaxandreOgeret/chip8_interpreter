// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_REGISTER_H
#define CHIP8_REGISTER_H

#include <limits>
#include <memory>
#include <type_traits>

template<class T>
class Register {
public:
  Register();
  void poke(T value);
  T peek();
  void increment(short int number = 1);
  void decrement(short int number = 1);

private:
  T value_;
};

template<class T>
Register<T>::Register() {
  value_ = 0x0;
}
template<class T>
void Register<T>::poke(T value) {
  value_ = value;
}
template<class T>
T Register<T>::peek() {
  return value_;
}

template<class T>
void Register<T>::increment(short int number) {
  value_ += number;
}

template<class T>
void Register<T>::decrement(short int number) {
  value_ -= number;
}

#endif//CHIP8_REGISTER_H