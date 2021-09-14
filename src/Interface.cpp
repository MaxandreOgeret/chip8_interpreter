// (c) 2021 Maxandre Ogeret
// Licensed under MIT License

#include "Interface.h"

Interface::Interface(const std::shared_ptr<reg::RegisterManager> & registers, bool hidden)
    : registers_(registers) {

  if (SDL_Init(SDL_INIT_AUDIO != 0) || SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Unable to initialize rendering engine.");
  }

  want_.freq = SAMPLE_RATE;
  want_.format = AUDIO_S16SYS;
  want_.channels = 1;
  want_.samples = 2048;
  want_.callback = Interface::forward_audio_callback;
  want_.userdata = &sound_userdata_;

  if (SDL_OpenAudio(&want_, &have_) != 0)
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
  if (want_.format != have_.format)
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to get the desired AudioSpec");

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

void Interface::poll_events() { SDL_PollEvent(&events_); }

bool Interface::requests_close() const {
  return events_.type == SDL_WINDOWEVENT && events_.window.event == SDL_WINDOWEVENT_CLOSE;
}

void Interface::clear() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void Interface::draw_pixel(unsigned short x, unsigned short y, bool on) {
  if (on) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  }
  SDL_Rect rect = {normalize_x(x), normalize_y(y), SIZE_MULTIPLIER_, SIZE_MULTIPLIER_};
  SDL_RenderFillRect(renderer, &rect);
}

void Interface::render() { SDL_RenderPresent(renderer); }

bool Interface::is_pixel_on(int x, int y) {
  p_ = (uint8_t *) SDL_GetWindowSurface(window)->pixels +
       normalize_y(y) * SDL_GetWindowSurface(window)->pitch + normalize_x(x) * bpp_;
  SDL_GetRGB(*(uint32_t *) p_, SDL_GetWindowSurface(window)->format, &rgb_.r, &rgb_.g, &rgb_.b);
  return rgb_.r != 0;
}

void Interface::get_keys() {
  if (events_.type == SDL_KEYDOWN && !events_.key.repeat) {
    pressed_key_ = events_.key.keysym.sym;
  } else {
    pressed_key_ = 0;
  }
}

bool Interface::is_pressed(uint8_t key) const {
  if (events_.key.repeat) { return false; }

  switch (key) {
    case 0x1:
      return pressed_key_ == SDLK_1;
    case 0x2:
      return pressed_key_ == SDLK_2;
    case 0x3:
      return pressed_key_ == SDLK_3;
    case 0xc:
      return pressed_key_ == SDLK_4;
    case 0x4:
      return pressed_key_ == SDLK_q;
    case 0x5:
      return pressed_key_ == SDLK_w;
    case 0x6:
      return pressed_key_ == SDLK_e;
    case 0xd:
      return pressed_key_ == SDLK_r;
    case 0x7:
      return pressed_key_ == SDLK_a;
    case 0x8:
      return pressed_key_ == SDLK_s;
    case 0x9:
      return pressed_key_ == SDLK_d;
    case 0xe:
      return pressed_key_ == SDLK_f;
    case 0xa:
      return pressed_key_ == SDLK_z;
    case 0x0:
      return pressed_key_ == SDLK_x;
    case 0xb:
      return pressed_key_ == SDLK_c;
    case 0xf:
      return pressed_key_ == SDLK_v;
    default:
      return false;
  }
}

uint8_t Interface::get_any_pressed() const {

  if (pressed_key_ == SDLK_1) {
    return 0x1;
  } else if (pressed_key_ == SDLK_2) {
    return 0x2;
  } else if (pressed_key_ == SDLK_3) {
    return 0x3;
  } else if (pressed_key_ == SDLK_4) {
    return 0xC;
  } else if (pressed_key_ == SDLK_q) {
    return 0x4;
  } else if (pressed_key_ == SDLK_w) {
    return 0x5;
  } else if (pressed_key_ == SDLK_e) {
    return 0x6;
  } else if (pressed_key_ == SDLK_r) {
    return 0xD;
  } else if (pressed_key_ == SDLK_a) {
    return 0x7;
  } else if (pressed_key_ == SDLK_s) {
    return 0x8;
  } else if (pressed_key_ == SDLK_d) {
    return 0x9;
  } else if (pressed_key_ == SDLK_f) {
    return 0xE;
  } else if (pressed_key_ == SDLK_z) {
    return 0xA;
  } else if (pressed_key_ == SDLK_x) {
    return 0x0;
  } else if (pressed_key_ == SDLK_c) {
    return 0xB;
  } else if (pressed_key_ == SDLK_v) {
    return 0xF;
  } else {
    return 0x10;
  }
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

void Interface::trigger_buzzer() {
  if (registers_->st_.peek() > 0) {
    SDL_PauseAudio(0);
  } else {
    SDL_PauseAudio(1);
  }
}

void Interface::simulate_keypress(SDL_Keycode key) {
  SDL_Event sdlevent = {};
  sdlevent.type = SDL_KEYDOWN;
  sdlevent.key.keysym.sym = key;
  SDL_PushEvent(&sdlevent);
}
