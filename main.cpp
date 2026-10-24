#include <cmath>
#include <raylib.h>
#include "ball.hpp"
#include "paddle.hpp"
#include <string>

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 700;
constexpr int PADDING = 40;
constexpr int FONT_SIZE = 60;

static Ball ball;
static Paddle left_paddle;
static Paddle right_paddle;
int score_left, score_right;

void Load() {
  ball.reset();
  ball.rect.width = BALL_SIZE;
  ball.rect.height = BALL_SIZE;
  ball.color = WHITE;

  left_paddle = {
    .rect={.x=PADDING, .y=(HEIGHT - PADDLE_HEIGHT)/2.0f,
           .width=PADDLE_WIDTH, .height=PADDLE_HEIGHT},
    .speed=PADDLE_SPEED, .color=WHITE
  };

  right_paddle = {
    .rect={.x=WIDTH - PADDING - PADDLE_WIDTH, .y=(HEIGHT - PADDLE_HEIGHT)/2.0f,
           .width=PADDLE_WIDTH, .height=PADDLE_HEIGHT},
    .speed=PADDLE_SPEED, .color=WHITE
  };
}

void Draw()
{
  DrawRectangleRec(ball.rect, ball.color);
  DrawRectangleRec(left_paddle.rect, left_paddle.color);
  DrawRectangleRec(right_paddle.rect, right_paddle.color);

  DrawLineDashed({WIDTH/2.0f, 0}, {WIDTH/2.0f, HEIGHT}, 20, 10, WHITE);

  auto score_left_str = std::to_string(score_left);
  DrawText(score_left_str.c_str(), WIDTH/2 - MeasureText(score_left_str.c_str(), FONT_SIZE) - PADDING,
           PADDING, FONT_SIZE, WHITE);

  auto score_right_str = std::to_string(score_right);
  DrawText(score_right_str.c_str(), WIDTH/2 + PADDING, PADDING, FONT_SIZE, WHITE);
}

bool check_collision(Rectangle &rect1, Rectangle &rect2)
{
  if (rect1.x + rect1.width < rect2.x ||
    rect2.x + rect2.width < rect1.x ||
    rect1.y + rect1.height < rect2.y ||
    rect2.y + rect2.height < rect1.y) return false;
  return true;
}

void Update(float dt)
{
  ball.update(dt, []{
    score_left += 1;
  }, []{
    score_right += 1;
  });

  if (check_collision(ball.rect, left_paddle.rect)) {
    ball.rect.x = left_paddle.rect.x + left_paddle.rect.width;
    ball.speed.x = std::fabsf(ball.speed.x);
  }

  if (check_collision(ball.rect, right_paddle.rect)) {
    ball.rect.x = right_paddle.rect.x - right_paddle.rect.width;
    ball.speed.x = -std::fabsf(ball.speed.x);
  }

  // W/S -> left
  if (IsKeyDown(KEY_W)) left_paddle.up(dt);
  if (IsKeyDown(KEY_S)) left_paddle.down(dt);

  // up/down arrow -> right
  if (IsKeyDown(KEY_UP)) right_paddle.up(dt);
  if (IsKeyDown(KEY_DOWN)) right_paddle.down(dt);
}

int main()
{
  InitWindow(WIDTH, HEIGHT, "Ping Pong");
  SetTargetFPS(180);
  Load();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));
    float dt = GetFrameTime();
    Draw();
    Update(dt);
    EndDrawing();
  }
}

// TODO: audio
// TODO: pausing
// TODO: custom font
