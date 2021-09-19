// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Interface.h"

Interface::Interface(const std::shared_ptr<reg::RegisterManager> & registers, bool hidden)
    : registers_(registers) {

  if (SDL_Init(SDL_INIT_AUDIO != 0) || SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Unable to initialize rendering engine.");
  }

  SDL_zero(want_);
  want_.freq = SAMPLE_RATE;
  want_.format = AUDIO_S16SYS;
  want_.channels = 1;
  want_.samples = 100;
  want_.callback = Interface::forward_audio_callback;
  want_.userdata = &sound_userdata_;

  if (SDL_OpenAudio(&want_, &have_) != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
  }
  if (want_.format != have_.format) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");
  }

  window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SIZE_X_ * SIZE_MULTIPLIER_, SIZE_Y_ * SIZE_MULTIPLIER_,
                            hidden ? SDL_WINDOW_HIDDEN : 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  bpp_ = SDL_GetWindowSurface(window)->format->BytesPerPixel;
  SDL_Delay(1000);
}

Interface::~Interface() {
  SDL_CloseAudio();
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Interface::poll_events() {
  SDL_PollEvent(&events_);
}

bool Interface::requests_close() const {
  return events_.type == SDL_WINDOWEVENT && events_.window.event == SDL_WINDOWEVENT_CLOSE;
}

void Interface::clear() {
  for (u_int8_t x = 0; x < SIZE_X_; x++) {
    for (u_int8_t y = 0; y < SIZE_Y_; y++) { set_pixel_state(x, y, false); }
  }
  render();
}

void Interface::set_pixel_state(unsigned short x, unsigned short y, bool state) {
  screen_memory_[x][y] = state;
}

void Interface::draw_pixel(unsigned short x, unsigned short y, bool state) {
  if (state) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  }
  SDL_Rect rect = {normalize_x(x), normalize_y(y), SIZE_MULTIPLIER_, SIZE_MULTIPLIER_};
  SDL_RenderFillRect(renderer, &rect);
}

void Interface::render() {
  for (u_int8_t x = 0; x < SIZE_X_; x++) {
    for (u_int8_t y = 0; y < SIZE_Y_; y++) { draw_pixel(x, y, screen_memory_[x][y]); }
  }
  SDL_RenderPresent(renderer);
}

bool Interface::is_pixel_on(int x, int y) {
  if (x >= SIZE_X_ || y >= SIZE_Y_) {
    throw std::runtime_error("Tried to get the value of an out of bound pixel.");
  }
  return screen_memory_[x][y];
}

void Interface::get_keys() {
  key_state_ = SDL_GetKeyboardState(NULL);
}

bool Interface::is_pressed(uint8_t key) const {
  switch (key) {
    case 0x1:
      return key_state_[SDL_SCANCODE_1];
    case 0x2:
      return key_state_[SDL_SCANCODE_2];
    case 0x3:
      return key_state_[SDL_SCANCODE_3];
    case 0xc:
      return key_state_[SDL_SCANCODE_4];
    case 0x4:
      return key_state_[SDL_SCANCODE_Q];
    case 0x5:
      return key_state_[SDL_SCANCODE_W];
    case 0x6:
      return key_state_[SDL_SCANCODE_E];
    case 0xd:
      return key_state_[SDL_SCANCODE_R];
    case 0x7:
      return key_state_[SDL_SCANCODE_A];
    case 0x8:
      return key_state_[SDL_SCANCODE_S];
    case 0x9:
      return key_state_[SDL_SCANCODE_D];
    case 0xe:
      return key_state_[SDL_SCANCODE_F];
    case 0xa:
      return key_state_[SDL_SCANCODE_Z];
    case 0x0:
      return key_state_[SDL_SCANCODE_X];
    case 0xb:
      return key_state_[SDL_SCANCODE_C];
    case 0xf:
      return key_state_[SDL_SCANCODE_V];
    default:
      return false;
  }
}

uint8_t Interface::get_any_pressed() const {
  if (key_state_[SDL_SCANCODE_1]) { return 0x1; }
  if (key_state_[SDL_SCANCODE_2]) { return 0x2; }
  if (key_state_[SDL_SCANCODE_3]) { return 0x3; }
  if (key_state_[SDL_SCANCODE_4]) { return 0xc; }
  if (key_state_[SDL_SCANCODE_Q]) { return 0x4; }
  if (key_state_[SDL_SCANCODE_W]) { return 0x5; }
  if (key_state_[SDL_SCANCODE_E]) { return 0x6; }
  if (key_state_[SDL_SCANCODE_R]) { return 0xd; }
  if (key_state_[SDL_SCANCODE_A]) { return 0x7; }
  if (key_state_[SDL_SCANCODE_S]) { return 0x8; }
  if (key_state_[SDL_SCANCODE_D]) { return 0x9; }
  if (key_state_[SDL_SCANCODE_F]) { return 0xe; }
  if (key_state_[SDL_SCANCODE_Z]) { return 0xa; }
  if (key_state_[SDL_SCANCODE_X]) { return 0x0; }
  if (key_state_[SDL_SCANCODE_C]) { return 0xb; }
  if (key_state_[SDL_SCANCODE_V]) { return 0xf; }
  return 0x10;
}

unsigned short int Interface::normalize_x(unsigned short int x) const {
  return (x % SIZE_X_) * SIZE_MULTIPLIER_;
}

unsigned short int Interface::normalize_y(unsigned short int y) const {
  return (y % SIZE_Y_) * SIZE_MULTIPLIER_;
}

void Interface::audio_callback(void * user_data, Uint8 * raw_buffer, int bytes) {
  audio_buffer_ = reinterpret_cast<Sint16 *>(raw_buffer);
  sample_length_ = bytes / 2;// 2 bytes per sample for AUDIO_S16SYS
  int & sample_nr(*(int *) user_data);

  for (int i = 0; i < sample_length_; i++, sample_nr++) {
    double time = (double) sample_nr / (double) SAMPLE_RATE;
    audio_buffer_[i] = static_cast<Sint16>(
            AMPLITUDE * (2 * (2 * floor(220.0f * time) - floor(2 * 220.0f * time)) + 1));
  }
}

void Interface::forward_audio_callback(void * user_data, Uint8 * raw_buffer, int bytes) {
  static_cast<Interface *>(user_data)->audio_callback(user_data, raw_buffer, bytes);
}

void Interface::toogle_buzzer() {
  if (registers_->st_.peek() > 0) {
    SDL_PauseAudio(0);
  } else {
    SDL_PauseAudio(1);
  }
}