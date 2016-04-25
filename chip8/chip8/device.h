uint32_t system_ticks(void);

/* device */
typedef struct device* device_t;
device_t create_device(void);
void destroy_device(device_t dev);

/* monitor dev */
#define WE_QUIT 0x100   /* quit window event */

typedef struct window* window_t;
window_t create_window(device_t dev);
void destroy_window(window_t window);
void window_render(window_t window, uint8_t *data, size_t n);
int window_poll_event(window_t dev);

/* sound dev */
typedef struct audio* audio_t;
audio_t create_audio(device_t dev);
void destroy_audio(audio_t audio);
void audio_beep(audio_t audio, int pause_on);

/* keyboard */
typedef struct keyboard* keyboard_t;
keyboard_t create_keyboard(device_t dev);
void destroy_keyboard(keyboard_t kb);
void keyboard_press_process(keyboard_t kb, uint8_t *keys, int wait_key);