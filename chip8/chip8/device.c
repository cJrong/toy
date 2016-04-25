#include <SDL2/SDL.h>
#include <malloc.h>
#include <assert.h>

#ifdef _WIN32
#pragma comment(lib, "SDL2.lib")
#endif

uint32_t system_ticks(void)
{
    return SDL_GetTicks();
}

struct device {
    void *dummy;
};

struct device* create_device()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    return (void*)1024;
}

void destroy_device(struct device* dev)
{
    (void)dev;
    SDL_Quit();
}

struct window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
};

struct window* create_window(struct device* dv)
{
    struct window* window = malloc(sizeof(struct window));
    if (!window) {
        return NULL;
    }

    window->window = SDL_CreateWindow("chip8 emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        320,
        SDL_WINDOW_SHOWN);
    if (!window->window) {
        goto window;
    }

    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
    if (!window->renderer) {
        goto renderer;
    }

    window->texture = SDL_CreateTexture(window->renderer,
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING,
        64,
        32);
    
    if (!window->texture) {
        goto texture;
    }

    return window;
texture:
    SDL_DestroyTexture(window->texture);
renderer:
    SDL_DestroyRenderer(window->renderer);
window:
    SDL_DestroyWindow(window->window);
    free(window);
    return NULL;
}

void destroy_window(struct window* window)
{
    assert(window != NULL);
    SDL_DestroyTexture(window->texture);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

void window_render(struct window* window, uint8_t *data, size_t n)
{
    uint32_t *pixels;
    int pitch;
    size_t i;

    SDL_LockTexture(window->texture, NULL, (void **)&pixels, &pitch);
    for (i = 0; i < n; i++) {
        pixels[i] = data[i] ? 0xFFFFFFFF : 0x00000000;
    }
    SDL_UnlockTexture(window->texture);

    SDL_RenderClear(window->renderer);
    SDL_RenderCopy(window->renderer, window->texture, NULL, NULL);
    SDL_RenderPresent(window->renderer);
}

int window_poll_event(struct window* window)
{
    SDL_PollEvent(&window->event);
    return window->event.type;
}

struct audio {
    float tone_pos;
    float tone_inc;
    SDL_AudioSpec* spec;
    SDL_AudioDeviceID audio_dev;
};

static void feed(void *udata, uint8_t* stream, int len)
{
    struct audio* audio = (struct audio *) udata;
    int i;

    for (i = 0; i < len; i++) {
        stream[i] = (uint8_t)(sinf(audio->tone_pos) + 127);
        audio->tone_pos += audio->tone_inc;
    }
}

struct audio* create_audio(struct device* dev)
{
    struct audio* audio = malloc(sizeof(struct audio));
    if (!audio) {
        return NULL;
    }
   
    audio->tone_pos = 0;
    audio->tone_inc = (float)(2.0 * 3.14159 * 1000 / 44100.0);

    SDL_AudioSpec* spec = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));
    if (!spec) {
        free(audio);
        return NULL;
    }

    spec->freq = 44100;
    spec->format = AUDIO_U8;
    spec->channels = 2;
    spec->samples = 4096;
    spec->callback = &feed;
    spec->userdata = audio;
    
    audio->spec = spec;
    audio->audio_dev = SDL_OpenAudioDevice(NULL, 0, spec, NULL,
        SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    return audio;
}

void destroy_audio(struct audio* audio)
{
    assert(audio != NULL);
    SDL_CloseAudioDevice(audio->audio_dev);
    free(audio->spec);
    free(audio);
}

void audio_beep(struct audio* audio, int pause_on)
{
    SDL_PauseAudioDevice(audio->audio_dev, pause_on);
}

struct keyboard {
    void *dummy;
};

struct keyboard* create_keyboard(struct device dev)
{
    (void)dev;
    return (struct keyboard*)1024;
}

void destroy_keyboard(struct keyboard* kb)
{
    (void)kb;
}

void keyboard_press_process(struct keyboard* kb, uint8_t *keys, int wait_key)
{
    const uint8_t *sdl_keys;

    sdl_keys = SDL_GetKeyboardState(NULL);

    
    if (sdl_keys[SDL_SCANCODE_1]) {
        keys[0] = 1;
    } else {
        keys[0] = 0;
    }
    
    if (sdl_keys[SDL_SCANCODE_2]) {
        keys[1] = 1;
    } else {
        keys[1] = 0;
    }
    
    if (sdl_keys[SDL_SCANCODE_3]) {
        keys[2] = 1;
    } else {
        keys[2] = 0;
    }
    
    if (sdl_keys[SDL_SCANCODE_4]) {
        keys[3] = 1;
    } else {
        keys[3] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_Q]) {
        keys[4] = 1;
    } else {
        keys[4] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_W]) {
        keys[5] = 1;
    } else {
        keys[5] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_E]) {
        keys[6] = 1;
    } else {
        keys[6] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_R]) {
        keys[7] = 1;
    } else {
        keys[7] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_A]) {
        keys[8] = 1;
    } else {
        keys[8] = 0;
    }
   
    if (sdl_keys[SDL_SCANCODE_S]) {
        keys[9] = 1;
    } else {
        keys[9] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_D]) {
        keys[10] = 1;
    } else {
        keys[10] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_F]) {
        keys[11] = 1;
    } else {
        keys[11] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_Z]) {
        keys[12] = 1;
    } else {
        keys[12] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_X]) {
        keys[13] = 1;
    } else {
        keys[13] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_C]) {
        keys[14] = 1;
    } else {
        keys[14] = 0;
    }

    if (sdl_keys[SDL_SCANCODE_V]) {
        keys[15] = 1;
    } else {
        keys[15] = 0;
    }
}