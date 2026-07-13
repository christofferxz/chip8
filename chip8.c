#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

// SDL object
typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_t;

// Emulator configuration object
typedef struct {
  uint32_t window_width;  // SDL window width
  uint32_t window_height; // SDL window height
  uint32_t fg_color;  // Foreground color RGBA8888
  uint32_t bg_color;  // Background color RGBA8888
  uint32_t scale_factor;  // Amount to scale a CHIP8 pixel by e.g. 20x will be a 20x larger window
} config_t;

// Emulator states
typedef enum {
  QUIT,
  RUNNING,
  PAUSED,
} emulator_state_t;

// CHIP8 Machine object
typedef struct {
  emulator_state_t state;
} chip8_t;

// Initialize SDL
bool initialize_sdl(sdl_t *sdl, const config_t config) {
  // SDL_Init return 1 for success and 0 for error
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
    SDL_Log("Could not initialize SDL! %s\n", SDL_GetError());
    return false; // Error
  };

  // Create SDL window
  sdl->window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                config.window_width * config.scale_factor,
                                config.window_height * config.scale_factor,
                                0);
  if (!sdl->window) {
    SDL_Log("Could not create SDL window %s\n", SDL_GetError());
    return false; // Error
  };

  // Create SDL renderer
  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
  if (!sdl->renderer) {
    SDL_Log("Could not create SDL renderer %s\n", SDL_GetError());
    return false; // Error
  };

  return true;  // Success
};

// Set up configurations from arguments
bool set_config_from_args(config_t *config, const int argc, char **argv) {
  // Set defaults
  *config = (config_t) {
    .window_width = 64, // CHIP8 original X resolution
    .window_height = 32, // CHIP8 original Y resolution
    .fg_color = 0xFFFF00FF, // YELLOW
    .bg_color = 0x00000000, // BLACK
    .scale_factor = 20, // Default resoultion will be 1280x640
  };

  // Skip compiler unused variables (argc/argv) error
  for (int i = 1; i < argc; i++) {
    (void) argv[i];
  };

  return true; // Success
};

// Initialize CHIP8 machine
bool init_chip8(chip8_t *chip8) {
  chip8->state = RUNNING; // Default machine state to running
  return true;  // Success
};

// Close SDL
void close_sdl(const sdl_t sdl) {
  SDL_DestroyRenderer(sdl.renderer);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
};

void clear_screen(const sdl_t sdl, const config_t config) {
  const uint8_t r = (config.bg_color >> 24) & 0xFF;
  const uint8_t g = (config.bg_color >> 16) & 0xFF;
  const uint8_t b = (config.bg_color >> 8) & 0xFF;
  const uint8_t a = (config.bg_color >> 0) & 0xFF;

  SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
  SDL_RenderClear(sdl.renderer);
};

// Update window with any changes
void update_screen(const sdl_t sdl) {
  SDL_RenderPresent(sdl.renderer);
};

void handle_input(chip8_t *chip8) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        // Exit window;
        chip8->state = QUIT; // Wil exit main emulator loop
        return;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            // Escape key; Exit window & End program
            chip8->state = QUIT;
            return;
          default:
            break;
        };
        break;
      case SDL_KEYUP:
        break;
      default:
        break;
    };
  };
};

// Main things
int main(int argc, char **argv) {
  // Initialize configurations
  config_t config = {0};
  if(!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);

  // Initialize SDL
  sdl_t sdl = {0};
  if (!initialize_sdl(&sdl, config)) exit(EXIT_FAILURE);

  // Initialize CHIP8 machine
  chip8_t chip8 = {0};
  if (!init_chip8(&chip8)) exit(EXIT_FAILURE);

  // Initial screen clear to background color
  clear_screen(sdl, config);

  // Emulator loop
  while (chip8.state != QUIT) {
    handle_input(&chip8);

    // Emulate CHIP8 Instructions
    // ...

    // Delay for approximately 60hz/60fps (16.67ms)
    SDL_Delay(16);

    // Update window with changes
    update_screen(sdl);
  };

  // Quit SDL
  close_sdl(sdl);

  exit(EXIT_SUCCESS);
};