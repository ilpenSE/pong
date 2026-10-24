#pragma once
#include <raylib.h>

constexpr int BALL_SIZE = 20;
constexpr int BALL_SPEED = 300;

struct Ball {
  template <typename FL, typename FR>
  void update(float dt, FL onLeftScore, FR onRightScore)
  {
    rect.x += speed.x * dt;
    rect.y += speed.y * dt;

    if (rect.x <= 0) {
      // Score for right
      reset();
      onRightScore();
    } else if (rect.x + rect.width >= GetScreenWidth()) {
      // Score for left
      reset();
      speed.x *= -1.0f;
      onLeftScore();
    }

    if (rect.y <= 0) {
      rect.y = 0;
      speed.y *= -1;
    } else if (rect.y + rect.height >= GetScreenHeight()) {
      rect.y = GetScreenHeight() - rect.height;
      speed.y *= -1;
    }
  }

  void reset() {
    rect.x = (GetScreenWidth() - BALL_SIZE)/2.0f;
    rect.y= (GetScreenHeight() - BALL_SIZE)/2.0f;
    speed = {BALL_SPEED, BALL_SPEED};
  }

  Rectangle rect;
  Vector2 speed;
  Color color;
};
