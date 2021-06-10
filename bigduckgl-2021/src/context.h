#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "query.h"

struct ContextParameters {
    uint32_t width = 1280;
    uint32_t height = 720;
    std::string title = "<Window title>";
    int gl_major = -1; // highest available
    int gl_minor = -1; // highest available
    int resizable = GLFW_TRUE;
    int visible = GLFW_TRUE;
    int decorated = GLFW_TRUE;
    int floating = GLFW_FALSE;
    int maximised = GLFW_FALSE;
    int gl_debug_context = GLFW_TRUE;
    uint32_t swap_interval = 1; // 0 = no vsync, 1 = 60fps, 2 = 30fps, etc
    std::string font_ttf_filename;
    uint32_t font_size_pixels = 13;
};

// Initialize and hold a GLFW/GL context + window.
class Context {
private:
    Context();
    ~Context();

public:
    // initialize context (using given params)
    static Context& init(const ContextParameters& params = ContextParameters());
    static Context& instance();

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context& operator=(const Context&&) = delete;

    // query if window should be closed (for use in main loop)
    static bool running();
    // finish current frame
    static void swap_buffers();
    // get last frame's time in ms
    static double frame_time();
    static void screenshot(const std::string& filepath);

    // modify
    static void show();
    static void hide();
    static void resize(int w, int h);
    static void set_title(const std::string& name);
    static void set_swap_interval(uint32_t interval); // 0: no vsync, 1: 60fps, 2: 30fps, ...
    static void capture_mouse(bool on);

    // access
    static glm::ivec2 resolution();
    static glm::vec2 mouse_pos();
    static bool key_pressed(int key); // key: GLFW_KEY_*
    static bool mouse_button_pressed(int button); // button: GLFW_MOUSE_BUTTON_*

    // installable user callbacks (see https://www.glfw.org/docs/latest/input_guide.html)
    static void set_keyboard_callback(void (*fn)(int key, int scancode, int action, int mods));
    static void set_mouse_callback(void (*fn)(double xpos, double ypos));
    static void set_mouse_button_callback(void (*fn)(int button, int action, int mods));
    static void set_mouse_scroll_callback(void (*fn)(double xoffset, double yoffset));
    static void set_resize_callback(void (*fn)(int w, int h));

    // translate GL macro names (GLint/GLenum) to string or vice versa
    static std::string enum_to_string(GLint value);
    static GLint string_to_enum(const std::string& value);

    // data
    GLFWwindow* glfw_window;
    double last_t, curr_t;
    static std::string base_path;
    std::shared_ptr<TimerQuery> cpu_timer, frame_timer;
    std::shared_ptr<TimerQueryGL> gpu_timer;
};
