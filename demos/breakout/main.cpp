#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "engine/engine.h"

namespace {

using engine::core::Clock;
using engine::core::Timestep;
using engine::input::Input;
using engine::input::KeyCode;
using engine::math::Clamp;
using engine::math::Matrix4;
using engine::math::Vector2;
using engine::renderer::Color;
using engine::renderer::PrimitiveRenderer;
using engine::renderer::RenderCommand;

constexpr float kScreenWidth = 960.0f;
constexpr float kScreenHeight = 540.0f;

struct Brick {
  Vector2 position;
  Vector2 size;
  Color color;
  bool alive = true;
};

struct Ball {
  Vector2 position;
  Vector2 velocity;
  float radius = 8.0f;
  bool stuck_to_paddle = true;
};

bool CheckCircleAabb(const Vector2& center, float radius,
                     const Vector2& box_pos, const Vector2& box_size,
                     Vector2* out_closest) {
  float closest_x = Clamp(center.x, box_pos.x, box_pos.x + box_size.x);
  float closest_y = Clamp(center.y, box_pos.y, box_pos.y + box_size.y);
  *out_closest = Vector2(closest_x, closest_y);

  Vector2 diff = center - *out_closest;
  return diff.LengthSquared() <= (radius * radius);
}

void ResetLevel(std::vector<Brick>* bricks, Vector2* paddle_pos, Ball* ball,
                int* score, int* lives, bool* game_over, bool* won) {
  bricks->clear();
  const int rows = 5;
  const int cols = 12;
  const float padding = 6.0f;
  const float margin_x = 48.0f;
  const float top_y = kScreenHeight - 70.0f;
  const float brick_w = (kScreenWidth - 2.0f * margin_x - (cols - 1) * padding) / cols;
  const float brick_h = 22.0f;

  const Color row_colors[rows] = {
      Color::Red(), Color::Orange(), Color::Yellow(), Color::Green(), Color::Cyan()
  };

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      Brick b;
      b.position = Vector2(margin_x + c * (brick_w + padding),
                           top_y - r * (brick_h + padding));
      b.size = Vector2(brick_w, brick_h);
      b.color = row_colors[r];
      b.alive = true;
      bricks->push_back(b);
    }
  }

  *paddle_pos = Vector2(kScreenWidth * 0.5f - 60.0f, 36.0f);
  ball->stuck_to_paddle = true;
  ball->velocity = Vector2(220.0f, 320.0f);
  *score = 0;
  *lives = 3;
  *game_over = false;
  *won = false;
}

}  // namespace

int main(int argc, char** argv) {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("================================================");
  ENGINE_LOG_INFO("   Agentic Game Engine - Breakout Demo          ");
  ENGINE_LOG_INFO("   Controls: A/D or Left/Right, Space to Launch ");
  ENGINE_LOG_INFO("   Press R to Reset, Esc to Quit                ");
  ENGINE_LOG_INFO("================================================");

  int max_frames = 0;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--frames" && i + 1 < argc) {
      max_frames = std::stoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Breakout";
  props.width = static_cast<std::uint32_t>(kScreenWidth);
  props.height = static_cast<std::uint32_t>(kScreenHeight);
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Could not create window for Breakout demo.");
    return 1;
  }

  if (!PrimitiveRenderer::Initialize()) {
    ENGINE_LOG_ERROR("Could not initialize PrimitiveRenderer.");
    return 1;
  }

  RenderCommand::SetViewport(0, 0, window->GetWidth(), window->GetHeight());

  const Vector2 paddle_size(120.0f, 18.0f);
  const float paddle_speed = 520.0f;

  Vector2 paddle_pos;
  Ball ball;
  std::vector<Brick> bricks;
  int score = 0;
  int lives = 3;
  bool game_over = false;
  bool won = false;

  ResetLevel(&bricks, &paddle_pos, &ball, &score, &lives, &game_over, &won);

  Clock clock;
  int frame_count = 0;

  while (!window->ShouldClose()) {
    Timestep dt = clock.Tick();
    // Clamp delta time to avoid huge physics jumps on window drag/pause
    float dt_sec = std::min(dt.GetSeconds(), 0.05f);

    if (Input::IsKeyPressed(KeyCode::Escape)) {
      break;
    }

    if (Input::IsKeyPressed(KeyCode::R)) {
      ResetLevel(&bricks, &paddle_pos, &ball, &score, &lives, &game_over, &won);
      ENGINE_LOG_INFO("Breakout level reset!");
    }

    if (!game_over && !won) {
      // Paddle movement
      if (Input::IsKeyPressed(KeyCode::A) || Input::IsKeyPressed(KeyCode::Left)) {
        paddle_pos.x -= paddle_speed * dt_sec;
      }
      if (Input::IsKeyPressed(KeyCode::D) || Input::IsKeyPressed(KeyCode::Right)) {
        paddle_pos.x += paddle_speed * dt_sec;
      }
      paddle_pos.x = Clamp(paddle_pos.x, 12.0f, kScreenWidth - paddle_size.x - 12.0f);

      if (ball.stuck_to_paddle) {
        ball.position = Vector2(paddle_pos.x + paddle_size.x * 0.5f,
                                paddle_pos.y + paddle_size.y + ball.radius + 1.0f);
        if (Input::IsKeyPressed(KeyCode::Space)) {
          ball.stuck_to_paddle = false;
        }
      } else {
        ball.position += ball.velocity * dt_sec;

        // Wall collisions
        if (ball.position.x - ball.radius < 0.0f) {
          ball.position.x = ball.radius;
          ball.velocity.x = std::abs(ball.velocity.x);
        } else if (ball.position.x + ball.radius > kScreenWidth) {
          ball.position.x = kScreenWidth - ball.radius;
          ball.velocity.x = -std::abs(ball.velocity.x);
        }

        if (ball.position.y + ball.radius > kScreenHeight) {
          ball.position.y = kScreenHeight - ball.radius;
          ball.velocity.y = -std::abs(ball.velocity.y);
        }

        // Bottom out -> lose life
        if (ball.position.y + ball.radius < 0.0f) {
          --lives;
          ENGINE_LOG_INFO("Ball missed! Lives remaining: " + std::to_string(lives));
          if (lives <= 0) {
            game_over = true;
            ENGINE_LOG_WARN("GAME OVER! Final Score: " + std::to_string(score) + " (Press R to restart)");
          } else {
            ball.stuck_to_paddle = true;
            ball.velocity = Vector2(220.0f, 320.0f);
          }
        }

        // Paddle collision
        Vector2 closest;
        if (ball.velocity.y < 0.0f &&
            CheckCircleAabb(ball.position, ball.radius, paddle_pos, paddle_size, &closest)) {
          float paddle_center = paddle_pos.x + paddle_size.x * 0.5f;
          float hit_offset = (ball.position.x - paddle_center) / (paddle_size.x * 0.5f);
          hit_offset = Clamp(hit_offset, -1.0f, 1.0f);

          float speed = ball.velocity.Length();
          ball.velocity.x = hit_offset * speed * 0.75f;
          ball.velocity.y = std::sqrt(std::max(100.0f, speed * speed - ball.velocity.x * ball.velocity.x));
          ball.position.y = paddle_pos.y + paddle_size.y + ball.radius;
        }

        // Brick collisions
        int alive_count = 0;
        for (auto& brick : bricks) {
          if (!brick.alive) {
            continue;
          }
          ++alive_count;

          Vector2 brick_closest;
          if (CheckCircleAabb(ball.position, ball.radius, brick.position, brick.size, &brick_closest)) {
            brick.alive = false;
            --alive_count;
            score += 100;

            Vector2 diff = ball.position - brick_closest;
            if (std::abs(diff.x) > std::abs(diff.y)) {
              ball.velocity.x = -ball.velocity.x;
            } else {
              ball.velocity.y = -ball.velocity.y;
            }
            break;
          }
        }

        if (alive_count == 0) {
          won = true;
          ENGINE_LOG_INFO("YOU WIN! Final Score: " + std::to_string(score) + " (Press R to play again)");
        }
      }
    }

    // Render
    RenderCommand::SetClearColor(Color(0.07f, 0.08f, 0.12f, 1.0f));
    RenderCommand::Clear();

    auto ortho = Matrix4::Orthographic(0.0f, kScreenWidth, 0.0f, kScreenHeight, -1.0f, 1.0f);
    PrimitiveRenderer::BeginBatch(ortho);

    // Arena border
    PrimitiveRenderer::DrawRectOutline(4.0f, 4.0f, kScreenWidth - 8.0f, kScreenHeight - 8.0f,
                                       Color(0.22f, 0.26f, 0.35f, 1.0f), 2.0f);

    // Life indicators (top-left)
    for (int i = 0; i < lives; ++i) {
      PrimitiveRenderer::DrawCircle(Vector2(24.0f + i * 22.0f, kScreenHeight - 24.0f),
                                    7.0f, Color::White(), 16);
    }

    // Bricks
    for (const auto& brick : bricks) {
      if (brick.alive) {
        PrimitiveRenderer::DrawQuad(brick.position, brick.size, brick.color);
        PrimitiveRenderer::DrawRectOutline(brick.position.x, brick.position.y,
                                           brick.size.x, brick.size.y,
                                           Color(0.05f, 0.05f, 0.08f, 0.6f), 1.5f);
      }
    }

    // Paddle
    Color paddle_color = game_over ? Color::Red() : (won ? Color::Green() : Color::White());
    PrimitiveRenderer::DrawQuad(paddle_pos, paddle_size, paddle_color);

    // Ball
    PrimitiveRenderer::DrawCircle(ball.position, ball.radius, Color::Yellow(), 24);

    PrimitiveRenderer::EndBatch();

    window->OnUpdate();
    ++frame_count;

    if (max_frames > 0 && frame_count >= max_frames) {
      ENGINE_LOG_INFO("Breakout demo reached target frame limit: " + std::to_string(max_frames));
      break;
    }
  }

  PrimitiveRenderer::Shutdown();
  ENGINE_LOG_INFO("Breakout demo exited cleanly.");
  return 0;
}
