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
const int SAMPLE_RATE = 20000;

class Interface {
public:
  Interface(const std::shared_ptr<reg::RegisterManager> & registers, bool hidden = false);
  virtual ~Interface();

  /**
   * Polls SDL events
   */
  void poll_events();

  /**
   * @returns The user requested the window to close.
   */
  bool requests_close() const;

  /**
   * clears screen
   */
  void clear();

  /**
   * Set a pixel to an on/off state in the screen memory.
   * @param x
   * @param y
   * @param on
   */
  void set_pixel_state(unsigned short int x, unsigned short int y, bool on);

  /**
   * Prints screen memory to display.
   */
  void render();

  /**
   * Returns given pixel state from screen memory.
   * @param x
   * @param y
   * @return
   */
  bool is_pixel_on(int x, int y);

  /**
   * Update pressed keys vector.
   */
  void get_keys();

  /**
   * @param key
   * @return Is the key pressed.
   */
  bool is_pressed(uint8_t key) const;

  /**
   * @return The pressed key. If none returns 0x10.
   */
  uint8_t get_any_pressed() const;

  /**
   * Toggles the buzzer based on the sound timer value.
   */
  void toogle_buzzer();

  const unsigned short int SIZE_X_ = 64;
  const unsigned short int SIZE_Y_ = 32;
  const unsigned short int SIZE_MULTIPLIER_ = 20;

  int sample_length_ = 0;
  std::vector<std::vector<bool>> screen_memory_ =
          std::vector<std::vector<bool>>(SIZE_X_, std::vector<bool>(SIZE_Y_, false));

  /**
   * For testing/debugging
   * @param key
   */
  static void simulate_keypress(SDL_Keycode key);

private:
  const Uint8 * key_state_;
  std::shared_ptr<reg::RegisterManager> registers_;
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  int bpp_;
  SDL_Event events_;
  Uint8 * p_;
  SDL_AudioSpec want_, have_;
  int sound_userdata_ = 0;
  Sint16 * audio_buffer_ = nullptr;

  /**
   * Normalizes x coordinates given the size of the window.
   * @param x
   * @return
   */
  unsigned short int normalize_x(unsigned short int x) const;

  /**
   * Normalizes y coordinates given the size of the window.
   * @param y
   * @return
   */
  unsigned short int normalize_y(unsigned short int y) const;

  /**
   * Audio callback that SDL uses to fill the audio buffer.
   * @param user_data
   * @param raw_buffer
   * @param bytes
   */
  void audio_callback(void * user_data, Uint8 * raw_buffer, int bytes);

  /**
   * Static forward function used as callback. Calls audio_callback.
   * @param userdata
   * @param stream
   * @param len
   */
  static void forward_audio_callback(void * userdata, Uint8 * stream, int len);

  /**
   * Draws one pixel on the display.
   * @param x
   * @param y
   * @param state
   */
  void draw_pixel(unsigned short x, unsigned short y, bool state);
};


#endif//CHIP8_INTERFACE_H
