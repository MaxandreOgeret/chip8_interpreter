// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include <vector>
#include <cstdint>
#include <exception>

template <class T>
class Stack {
public:
  Stack(size_t max_size);
  void push_back(T value);
  T pop_back();
  T back();
  size_t size();

private:
  std::vector<T> values_ = {};
  size_t max_size_;
  T temp_;
};

template<class T>
Stack<T>::Stack(size_t max_size): max_size_(max_size) {}

template<class T>
void Stack<T>::push_back(T value) {
  if (values_.size() > max_size_) {
    throw std::runtime_error("Stack Overflow ("+ std::to_string(max_size_) + ")");
  }
  values_.push_back(value);
}

template<class T>
T Stack<T>::pop_back() {
  temp_ = values_.back();
  values_.pop_back();
  return temp_;
}

template<class T>
T Stack<T>::back() {
  return values_.back();
}

template<class T>
size_t Stack<T>::size() {
  values_.size();
}

#endif//CHIP8_STACK_H
