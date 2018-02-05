#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

GLFWwindow *init_main_window(const unsigned width, const unsigned height);
void fb_resize_cb(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
