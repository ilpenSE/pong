#include <cmath>
#include <cstdio>
#include <raylib.h>
#include <string>
#include <concepts>
#include <cstring>

// Constants
// Ball
constexpr int BALL_SIZE = 20;
constexpr int BALL_SPEED = 300;

// Paddle
constexpr int PADDLE_WIDTH = 20;
constexpr int PADDLE_HEIGHT = 130;
constexpr int PADDLE_SPEED = 280;

// General
constexpr int WIDTH = 1000;
constexpr int HEIGHT = 700;
constexpr int PADDING = 40;
constexpr int FONT_SIZE = 60;

struct Paddle {
  void up(float dt) {
    if (rect.y > 0) {
      rect.y -= speed * dt;
      if (rect.y <= 0) rect.y = 0;
    }
  }

  void down(float dt) {
    if (rect.y + rect.height < HEIGHT) {
      rect.y += speed * dt;
      if (rect.y + rect.height >= HEIGHT) {
        rect.y = HEIGHT - rect.height;
      }
    }
  }

  Rectangle rect;
  float speed;
  Color color;
};

struct Ball {
  void update(float dt, std::invocable<> auto&& onLeftScore, std::invocable<> auto&& onRightScore)
  {
    rect.x += speed.x * dt;
    rect.y += speed.y * dt;

    if (rect.x <= 0) {
      // Score for right
      reset();
      onRightScore();
    } else if (rect.x + rect.width >= WIDTH) {
      // Score for left
      reset();
      speed.x *= -1.0f;
      onLeftScore();
    }

    if (rect.y <= 0) {
      rect.y = 0;
      speed.y *= -1;
    } else if (rect.y + rect.height >= HEIGHT) {
      rect.y = HEIGHT - rect.height;
      speed.y *= -1;
    }
  }

  void reset() {
    rect.x = (WIDTH - BALL_SIZE)/2.0f;
    rect.y= (HEIGHT - BALL_SIZE)/2.0f;
    speed = {BALL_SPEED, BALL_SPEED};
  }

  Rectangle rect;
  Vector2 speed;
  Color color;
};

static bool paused = false;
static Ball ball;
static Paddle left_paddle;
static Paddle right_paddle;
static int score_left, score_right;
static Sound score_sound, pong_sound;

void Load();
void Update(float dt);
void Draw();
void Close();

const char *shift(int &argc, char **&argv) {
  return argc == 0 ? nullptr : (argc--, *argv++);
}

int main(int argc, char **argv)
{
  const char *program_name = shift(argc, argv);
  int fps = 60;

  const char *arg_cstr;
  while ((arg_cstr = shift(argc, argv)) != nullptr) {
    std::string_view arg = arg_cstr;
    if (arg == "--fps") {
      const char *fps_cstr = shift(argc, argv);
      if (!fps_cstr) {
        fprintf(stderr, "ERROR: --fps should have an integer!\n");
        return 1;
      }

      try {
        fps = std::stoi(fps_cstr);
      } catch (...) {
        fprintf(stderr, "ERROR: '%s' is not an integer.\n", fps_cstr);
        return 1;
      }
      printf("Setting target FPS to %d\n", fps);
    }
  }

  InitWindow(WIDTH, HEIGHT, "Ping Pong");
  SetTargetFPS(fps);
  Load();

  while (!WindowShouldClose()) {
    BeginDrawing();
      float dt = GetFrameTime();
      Update(dt);
      Draw();
    EndDrawing();
  }

  Close();
  CloseWindow();
}

bool check_collision(Rectangle &rect1, Rectangle &rect2)
{
  if (rect1.x + rect1.width < rect2.x ||
    rect2.x + rect2.width < rect1.x ||
    rect1.y + rect1.height < rect2.y ||
    rect2.y + rect2.height < rect1.y) return false;
  return true;
}

void Load() {
  InitAudioDevice();

  score_sound = LoadSound("sounds/score.ogg");
  pong_sound = LoadSound("sounds/pong.ogg");
  if (!IsSoundValid(score_sound)) {
    TraceLog(LOG_ERROR, "Score sound not found, sound won't be played");
  }
  if (!IsSoundValid(pong_sound)) {
    TraceLog(LOG_ERROR, "Pong sound not found, sound won't be played");
  }

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

void Update(float dt)
{
  if (IsKeyPressed(KEY_SPACE)) paused = !paused;
  if (paused) return;

  ball.update(dt, []{
    score_left += 1;
    PlaySound(score_sound);
  }, []{
    score_right += 1;
    PlaySound(score_sound);
  });

  if (check_collision(ball.rect, left_paddle.rect)) {
    ball.rect.x = left_paddle.rect.x + left_paddle.rect.width;
    ball.speed.x = std::fabsf(ball.speed.x);
    PlaySound(pong_sound);
  }

  if (check_collision(ball.rect, right_paddle.rect)) {
    ball.rect.x = right_paddle.rect.x - right_paddle.rect.width;
    ball.speed.x = -std::fabsf(ball.speed.x);
    PlaySound(pong_sound);
  }

  // W/S -> left
  if (IsKeyDown(KEY_W)) left_paddle.up(dt);
  if (IsKeyDown(KEY_S)) left_paddle.down(dt);

  // up/down arrow -> right
  if (IsKeyDown(KEY_UP)) right_paddle.up(dt);
  if (IsKeyDown(KEY_DOWN)) right_paddle.down(dt);
}

void Draw()
{
  ClearBackground(GetColor(0x181818FF));

  DrawRectangleRec(ball.rect, ball.color);
  DrawRectangleRec(left_paddle.rect, left_paddle.color);
  DrawRectangleRec(right_paddle.rect, right_paddle.color);

  DrawLineDashed({WIDTH/2.0f, 0}, {WIDTH/2.0f, HEIGHT}, 20, 10, WHITE);

  auto score_left_str = std::to_string(score_left);
  DrawText(score_left_str.c_str(), WIDTH/2 - MeasureText(score_left_str.c_str(), FONT_SIZE) - PADDING,
           PADDING, FONT_SIZE, WHITE);

  auto score_right_str = std::to_string(score_right);
  DrawText(score_right_str.c_str(), WIDTH/2 + PADDING, PADDING, FONT_SIZE, WHITE);

  if (paused) {
    const char *paused_str = "paused";
    DrawText(paused_str, (WIDTH - MeasureText(paused_str, FONT_SIZE))/2, PADDING*3, FONT_SIZE, WHITE);
  }
}

void Close()
{
  UnloadSound(score_sound);
  UnloadSound(pong_sound);
  CloseAudioDevice();
}
