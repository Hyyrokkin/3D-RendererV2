#pragma once
#include <stddef.h>

//Window properties
#define WINDOW_TITLE "3D Renderer"
constexpr size_t WINDOW_TITLE_MAX_LENGTH = 128;
constexpr float WINDOW_WIDTH = 1028.0f;
constexpr float WINDOW_HEIGHT = 720.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 1000.0f;
constexpr float FOV = 90.0f;

//Camera move speed
constexpr float VERT_MOVE_SPEED = 6.0f;
constexpr float HOR_MOVE_SPEED = 4.0f;
constexpr float TURN_SPEED = 2.5f;

#define RENDER_DEBUG
