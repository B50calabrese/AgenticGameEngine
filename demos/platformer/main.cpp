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

struct Platform {
  Vector2 position;
  Vector2 size;
  Color color;
  bool is_moving = false;
  float move_origin_x = 0.0f;
  float move_range = 0.0f;
  float move_speed = 0.0f;
};

struct Coin {
  Vector2 position;
  float radius = 10.0f;
  bool collected = false;
};

struct Player {
  Vector2 position;
  Vector2 velocity;
  Vector2 size = Vector2(28.0f, 40.0f);
  bool is_grounded = false;
};

bool AabbOverlap(const Vector2& a_pos, const Vector2& a_size,
                 const Vector2& b_pos, const Vector2& b_size) {
  return (a_pos.x < b_pos.x + b_size.x) && (a_pos.x + a_size.x > b_pos.x) &&
         (a_pos.y < b_pos.y + b_size.y) && (a_pos.y + a_size.y > b_pos.y);
}

void ResetLevel(Player* player, std::vector<Platform>* platforms,
                std::vector<Coin>* coins, int* score, bool* won) {
  player->position = Vector2(60.0f, 80.0f);
  player->velocity = Vector2::Zero();
  player->is_grounded = false;

  platforms->clear();
  // Ground segments (with a gap to jump over)
  platforms->push_back({Vector2(0.0f, 0.0f), Vector2(420.0f, 40.0f), Color(0.25f, 0.55f, 0.30f, 1.0f)});
  platforms->push_back({Vector2(520.0f, 0.0f), Vector2(440.0f, 40.0f), Color(0.25f, 0.55f, 0.30f, 1.0f)});

  // Stepping platforms
  platforms->push_back({Vector2(180.0f, 110.0f), Vector2(140.0f, 20.0f), Color(0.35f, 0.45f, 0.65f, 1.0f)});
  platforms->push_back({Vector2(380.0f, 190.0f), Vector2(140.0f, 20.0f), Color(0.35f, 0.45f, 0.65f, 1.0f)});
  platforms->push_back({Vector2(600.0f, 150.0f), Vector2(140.0f, 20.0f), Color(0.35f, 0.45f, 0.65f, 1.0f)});

  // Moving platform
  Platform moving;
  moving.position = Vector2(680.0f, 270.0f);
  moving.size = Vector2(120.0f, 20.0f);
  moving.color = Color::Orange();
  moving.is_moving = true;
  moving.move_origin_x = 680.0f;
  moving.move_range = 110.0f;
  moving.move_speed = 1.6f;
  platforms->push_back(moving);

  // High goal platform
  platforms->push_back({Vector2(120.0f, 340.0f), Vector2(200.0f, 24.0f), Color(0.55f, 0.35f, 0.75f, 1.0f)});
  platforms->push_back({Vector2(420.0f, 310.0f), Vector2(150.0f, 20.0f), Color(0.35f, 0.45f, 0.65f, 1.0f)});

  coins->clear();
  coins->push_back({Vector2(250.0f, 150.0f)});
  coins->push_back({Vector2(450.0f, 230.0f)});
  coins->push_back({Vector2(670.0f, 190.0f)});
  coins->push_back({Vector2(495.0f, 350.0f)});
  coins->push_back({Vector2(220.0f, 390.0f)});

  *score = 0;
  *won = false;
}

}  // namespace

int main(int argc, char** argv) {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("================================================");
  ENGINE_LOG_INFO("   Agentic Game Engine - Platformer Demo        ");
  ENGINE_LOG_INFO("   Controls: A/D or Left/Right, Space/W to Jump ");
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
  props.title = "Agentic Game Engine - 2D Platformer";
  props.width = static_cast<std::uint32_t>(kScreenWidth);
  props.height = static_cast<std::uint32_t>(kScreenHeight);
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Could not create window for Platformer demo.");
    return 1;
  }

  if (!PrimitiveRenderer::Initialize()) {
    ENGINE_LOG_ERROR("Could not initialize PrimitiveRenderer.");
    return 1;
  }

  RenderCommand::SetViewport(0, 0, window->GetWidth(), window->GetHeight());

  Player player;
  std::vector<Platform> platforms;
  std::vector<Coin> coins;
  int score = 0;
  bool won = false;

  ResetLevel(&player, &platforms, &coins, &score, &won);

  const float move_speed = 280.0f;
  const float jump_velocity = 540.0f;
  const float gravity = -1250.0f;

  Clock clock;
  int frame_count = 0;

  while (!window->ShouldClose()) {
    Timestep dt = clock.Tick();
    float dt_sec = std::min(dt.GetSeconds(), 0.05f);
    float elapsed = clock.GetTotalElapsedSeconds();

    if (Input::IsKeyPressed(KeyCode::Escape)) {
      break;
    }

    if (Input::IsKeyPressed(KeyCode::R)) {
      ResetLevel(&player, &platforms, &coins, &score, &won);
      ENGINE_LOG_INFO("Platformer level reset!");
    }

    // Update moving platforms
    for (auto& plat : platforms) {
      if (plat.is_moving) {
        plat.position.x = plat.move_origin_x +
                          std::sin(elapsed * plat.move_speed) * plat.move_range;
      }
    }

    // Horizontal input
    player.velocity.x = 0.0f;
    if (Input::IsKeyPressed(KeyCode::A) || Input::IsKeyPressed(KeyCode::Left)) {
      player.velocity.x -= move_speed;
    }
    if (Input::IsKeyPressed(KeyCode::D) || Input::IsKeyPressed(KeyCode::Right)) {
      player.velocity.x += move_speed;
    }

    // Jump
    if (player.is_grounded &&
        (Input::IsKeyPressed(KeyCode::Space) ||
         Input::IsKeyPressed(KeyCode::W) ||
         Input::IsKeyPressed(KeyCode::Up))) {
      player.velocity.y = jump_velocity;
      player.is_grounded = false;
    }

    // Apply gravity
    player.velocity.y += gravity * dt_sec;

    // Move & resolve X
    player.position.x += player.velocity.x * dt_sec;
    player.position.x = Clamp(player.position.x, 0.0f, kScreenWidth - player.size.x);
    for (const auto& plat : platforms) {
      if (AabbOverlap(player.position, player.size, plat.position, plat.size)) {
        if (player.velocity.x > 0.0f) {
          player.position.x = plat.position.x - player.size.x;
        } else if (player.velocity.x < 0.0f) {
          player.position.x = plat.position.x + plat.size.x;
        }
      }
    }

    // Move & resolve Y
    player.position.y += player.velocity.y * dt_sec;
    player.is_grounded = false;
    for (const auto& plat : platforms) {
      if (AabbOverlap(player.position, player.size, plat.position, plat.size)) {
        if (player.velocity.y < 0.0f) {
          player.position.y = plat.position.y + plat.size.y;
          player.velocity.y = 0.0f;
          player.is_grounded = true;
        } else if (player.velocity.y > 0.0f) {
          player.position.y = plat.position.y - player.size.y;
          player.velocity.y = 0.0f;
        }
      }
    }

    // Fall off bottom -> respawn
    if (player.position.y + player.size.y < -40.0f) {
      ENGINE_LOG_INFO("Fell into pit! Respawning...");
      ResetLevel(&player, &platforms, &coins, &score, &won);
    }

    // Collect coins
    int remaining_coins = 0;
    for (auto& coin : coins) {
      if (coin.collected) {
        continue;
      }
      Vector2 coin_box_pos(coin.position.x - coin.radius, coin.position.y - coin.radius);
      Vector2 coin_box_size(coin.radius * 2.0f, coin.radius * 2.0f);
      if (AabbOverlap(player.position, player.size, coin_box_pos, coin_box_size)) {
        coin.collected = true;
        score += 10;
        ENGINE_LOG_INFO("Coin collected! Score: " + std::to_string(score));
      } else {
        ++remaining_coins;
      }
    }

    if (!won && remaining_coins == 0) {
      won = true;
      ENGINE_LOG_INFO("ALL COINS COLLECTED! You win! (Press R to play again)");
    }

    // Render
    RenderCommand::SetClearColor(Color(0.08f, 0.11f, 0.18f, 1.0f));
    RenderCommand::Clear();

    auto ortho = Matrix4::Orthographic(0.0f, kScreenWidth, 0.0f, kScreenHeight, -1.0f, 1.0f);
    PrimitiveRenderer::BeginBatch(ortho);

    // Platforms
    for (const auto& plat : platforms) {
      PrimitiveRenderer::DrawQuad(plat.position, plat.size, plat.color);
      PrimitiveRenderer::DrawRectOutline(plat.position.x, plat.position.y,
                                         plat.size.x, plat.size.y,
                                         Color(1.0f, 1.0f, 1.0f, 0.25f), 1.5f);
    }

    // Coins (with subtle bobbing animation)
    for (const auto& coin : coins) {
      if (!coin.collected) {
        float bob = std::sin(elapsed * 4.0f + coin.position.x * 0.05f) * 4.0f;
        PrimitiveRenderer::DrawCircle(
            Vector2(coin.position.x, coin.position.y + bob),
            coin.radius, Color::Yellow(), 20);
      }
    }

    // Player
    Color player_color = won ? Color::Green() : Color::Cyan();
    PrimitiveRenderer::DrawQuad(player.position, player.size, player_color);
    PrimitiveRenderer::DrawRectOutline(player.position.x, player.position.y,
                                       player.size.x, player.size.y,
                                       Color::White(), 1.5f);

    PrimitiveRenderer::EndBatch();

    window->OnUpdate();
    ++frame_count;

    if (max_frames > 0 && frame_count >= max_frames) {
      ENGINE_LOG_INFO("Platformer demo reached target frame limit: " + std::to_string(max_frames));
      break;
    }
  }

  PrimitiveRenderer::Shutdown();
  ENGINE_LOG_INFO("Platformer demo exited cleanly.");
  return 0;
}
