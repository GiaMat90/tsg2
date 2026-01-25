#pragma once

#include "glfw_game.h"
#include "glfw_renderer.h"
#include "glfw_game_timer.h"
#include "glfw_input.h"
#include "glfw_window.h"
#include "glfw_event.h"
#include "glfw_camera.h"
#include "glfw_cursor.h"
#include "glfw_font.h"

using graphic_impl = glfw_game;
using window_impl = glfw_window;
using camera_impl = glfw_camera;
using renderer_impl = glfw_renderer;
using input_impl = glfw_input;
using game_event_impl = glfw_event;
using game_timer_impl = glfw_game_timer;
using cursor_impl = glfw_cursor;
using font_impl = glfw_font;
