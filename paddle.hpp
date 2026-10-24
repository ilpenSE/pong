#pragma once
#include <raylib.h>

constexpr int PADDLE_WIDTH = 20;
constexpr int PADDLE_HEIGHT = 130;
constexpr int PADDLE_SPEED = 280;

struct Paddle {
  void up(float dt) {
    if (rect.y > 0) {
      rect.y -= speed * dt;
      if (rect.y <= 0) rect.y = 0;
    }
  }

  void down(float dt) {
    auto screen_height = GetScreenHeight();
    if (rect.y + rect.height < screen_height) {
      rect.y += speed * dt;
      if (rect.y + rect.height >= screen_height) {
        rect.y = screen_height - rect.height;
      }
    }
  }

  Rectangle rect;
  float speed;
  Color color;
};
