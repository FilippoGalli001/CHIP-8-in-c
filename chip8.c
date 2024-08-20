#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;

} sdl_t;

// Emulator configuration
typedef struct{
    uint32_t window_width;
    uint32_t window_height;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t scale_factor;
} config_t;

// Emulator states
typedef enum{
    QUIT,
    RUNNING,
    PAUSED,
} emulator_state_t;

// CHIP8 Machine object
typedef struct{
    emulator_state_t state;
} chip8_t;

bool init_sdl(sdl_t *sdl, config_t config){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        SDL_Log("Could not initialize SDL subsystems!! %s\n", SDL_GetError());
        return false;
    }

    sdl->window = SDL_CreateWindow("CHIP-8 EMULATOR",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 
                                    config.window_width * config.scale_factor, config.window_height * config.scale_factor, 0);

    if (!sdl->window){
        SDL_Log("Could not create SDL window%s\n", SDL_GetError());
        return false;
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl->renderer){
        SDL_Log("Could not create SDL renderer%s\n", SDL_GetError());
        return false;
    }
    return true;
}

// Set up initial emulator configuration from passed args
bool set_config_from_args(config_t *config, int argc, char **argv){
    
    //Set default
    config->window_width = 64;
    config->window_height = 32;
    config->fg_color = 0xFFFFFFFF;
    config->bg_color = 0xFFFF00FF;
    config->scale_factor = 20;

    //Override default with passed arguments
    for (int i = 1; i < argc; i++){
        (void)argv[i];
    }

    return true;
}

void final_cleanup(const sdl_t sdl){
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

// Clear screen / SDL Window to background color
void clear_screen(const sdl_t sdl, const config_t config){
    const uint32_t r = (config.bg_color >> 24) & 0xFF;
    const uint32_t g = (config.bg_color >> 16) & 0xFF;
    const uint32_t b = (config.bg_color >> 8) & 0xFF;
    const uint32_t a = (config.bg_color >> 0) & 0xFF;
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
    SDL_RenderClear(sdl.renderer);
}

void update_screen(const sdl_t sdl) {
    SDL_RenderPresent(sdl.renderer);
}

void handle_input(chip8_t *chip8) {
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch (event.type)
        {
        case SDL_QUIT:
            chip8->state = QUIT; // Quit main emulator loop
            return;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        

        }
    }
}

bool init_chip8(chip8_t *chip8, const config_t config){
    return true;
}

int main(int argc, char **argv){

    // Initialize emulator config
    config_t config = {0};  
    if (!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);

    // Initialize SDL
    sdl_t sdl = {0};
    if (!init_sdl(&sdl, config)) exit(EXIT_FAILURE);

    // Initialize CHIP8 machine
    chip8_t chip8 = {0};
    if (!init_chip8(&chip8, config)) exit(EXIT_FAILURE);

    // Initialize screen clear to background color
    clear_screen(sdl, config);

    chip8.state = RUNNING;

    // Main emulator loop
    while(chip8.state != QUIT){

        // Handle user input
        handle_input(&chip8);
        // Delay for 60FPS (16.67 ms)
        SDL_Delay(16);
        // Update window with changes
        update_screen(sdl);
    }
    
    final_cleanup(sdl);
    
    exit(EXIT_SUCCESS);
}