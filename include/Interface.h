// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#ifndef CHIP8_INTERFACE_H
#define CHIP8_INTERFACE_H

#include "SDL.h"
#include "SDL_audio.h"

#include "iostream"
#include <cmath>
#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>

#include "register/RegisterManager.h"

const int AMPLITUDE = 500;
const int SAMPLE_RATE = 44100;

class Interface {
public:
  Interface(const std::shared_ptr<reg::RegisterManager> & registers, bool hidden = false);
  virtual ~Interface();
  void poll_events();
  bool requests_close() const;
  void clear();
  void set_pixel_state(unsigned short int x, unsigned short int y, bool on);
  void render();
  bool is_pixel_on(int x, int y);
  void get_keys();
  bool is_pressed(uint8_t key) const;
  uint8_t get_any_pressed() const;
  void toogle_buzzer();

  const unsigned short int SIZE_X_ = 64;
  const unsigned short int SIZE_Y_ = 32;
  const unsigned short int SIZE_MULTIPLIER_ = 20;
  std::vector<std::vector<bool>> screen_memory_ =
          std::vector<std::vector<bool>>(SIZE_X_, std::vector<bool>(SIZE_Y_, false));

  /**
   * For testing/debugging
   * @param key
   */
  static void simulate_keypress(SDL_Keycode key);

private:
  std::shared_ptr<reg::RegisterManager> registers_;
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  int bpp_;
  SDL_Color rgb_;
  SDL_Event events_;
  Uint8 * p_;
  SDL_Keycode pressed_key_;
  SDL_AudioSpec want_, have_;
  int sound_userdata_ = 0;
  Sint16 * audio_buffer_ = nullptr;
  int sample_length_ = 0;

  unsigned short int normalize_x(unsigned short int x) const;
  unsigned short int normalize_y(unsigned short int y) const;
  void audio_callback(void * user_data, Uint8 * raw_buffer, int bytes);
  static void forward_audio_callback(void * userdata, Uint8 * stream, int len);
  void draw_pixel(unsigned short x, unsigned short y, bool state);
};


#endif//CHIP8_INTERFACE_H
