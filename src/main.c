#include "snake.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

Vector2 keyDetection(KeyboardKey key1, KeyboardKey key2, KeyboardKey key3,
                     Vector2 accelerationVector, bool *isGrounded);
Rectangle IsGroundedDetection(Rectangle PlayerCollider,
                              Rectangle backgroundCollider, float *velocityY,
                              bool *isGrounded);
typedef struct Snake {
  Vector2 position;
  Vector2 size;
  Vector2 speed;
  Color color;
} Snake;

typedef struct Food {
  Vector2 position;
  Vector2 size;
  bool active;
  Color color;
} Food;

int main() {
  //----------------------------------------------------------------------------------
  // Initialization
  //----------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Snekken");

  ChangeDirectory("..");

  // Load background image
  Image background = LoadImage("assets/background.png");
  ImageResizeNN(&background, screenWidth, 30);
  Texture2D backgroundTexture = LoadTextureFromImage(background);
  UnloadImage(background);
  Rectangle backgroundCollider = {0, screenHeight - 30, backgroundTexture.width,
                                  backgroundTexture.height};

  //----------------------------------------------------------------------------------
  // Player 1
  //----------------------------------------------------------------------------------
  bool isPlayer1Grounded = true;
  Image player1Image = LoadImage("assets/EnterNameReworked.png");
  const int SpriteSize = 36 * 4;
  ImageResizeNN(&player1Image, SpriteSize * 12, SpriteSize);
  Texture2D player1Texture = LoadTextureFromImage(player1Image);
  Rectangle player1Collider = {screenWidth / 2.0f - 25,
                               backgroundCollider.y - player1Image.height,
                               player1Image.width, player1Image.height};
  Vector2 player1Velocity = {0.0f, 0.0f};
  Vector2 player1Acceleration = {0.0f, 0.0f};
  double player1HP = 100;
  int cooldownTimer1 = 100;
  int playAnimation = 0;
  int player1FrameCounter = 1;
  int player1AnimationTimer = 0;

  //----------------------------------------------------------------------------------
  // Player 2
  //----------------------------------------------------------------------------------
  bool isPlayer2Grounded = true;
  Image player2Image = LoadImage("assets/EnterNameReworked.png");
  ImageResizeNN(&player2Image, SpriteSize * 12, SpriteSize);
  Texture2D player2Texture = LoadTextureFromImage(player2Image);
  Rectangle player2Collider = {screenWidth / 2.0f - 25,
                               backgroundCollider.y - player2Image.height,
                               player2Image.width, player2Image.height};
  Vector2 player2Velocity = {0.0f, 0.0f};
  Vector2 player2Acceleration = {0.0f, 0.0f};
  double player2HP = 100;
  int cooldownTimer2 = 100;
  int playAnimation2 = 0;
  int player2FrameCounter = 1;
  int player2AnimationTimer = 0;

  // Debug mode and snake mode
  bool debugMode = true;
  bool snakeRunning = false;

  const double gravitationalForce = 4;

  // Friction coefficient
  const float friction = 0.99;
  // Snake Defines
  static int framesCounter = 0;
  static bool gameOver = false;
  static bool pause = false;

  static Food fruit = {0};
  static Snake snake[SNAKE_LENGTH] = {0};
  static Vector2 snakePosition[SNAKE_LENGTH] = {0};
  static bool allowMove = false;
  static Vector2 offset = {0};
  static int counterTail = 2;

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    cooldownTimer2 += 150 * dt;
    cooldownTimer1 += 150 * dt;

    //----------------------------------------------------------------------------------
    // Player 1
    //----------------------------------------------------------------------------------
    player1Acceleration = keyDetection(KEY_D, KEY_A, KEY_W, player1Acceleration,
                                       &isPlayer1Grounded);

    // Apply gravity if not grounded
    if (!isPlayer1Grounded) {
      player1Acceleration.y += gravitationalForce;
    }

    if (IsKeyPressed(KEY_V)) {
      playAnimation = 1;
    }

    if (IsKeyPressed(KEY_K)) {
      playAnimation2 = 1;
    }

    if (IsKeyPressed(KEY_X) || snakeRunning) {
      snakeRunning = true;

      UpdateDrawFrame();
    }

    if (IsKeyPressed(KEY_Y)) {
      snakeRunning = false;
    }

    // Update player1Velocity with player1Acceleration
    player1Velocity.x += player1Acceleration.x;
    player1Velocity.y += player1Acceleration.y;

    // Reset player1Acceleration
    player1Acceleration = (Vector2){0.0f, 0.0f};

    // Apply friction to player1Velocity
    player1Velocity.x *= friction;

    // Update position with player1Velocity
    if (player1Collider.x > -50 || player1Velocity.x >= 0) {
      player1Collider.x += player1Velocity.x * dt;
    }
    player1Collider.y += player1Velocity.y * dt;

    player1Collider =
        IsGroundedDetection(player1Collider, backgroundCollider,
                            &player1Velocity.y, &isPlayer1Grounded);

    //----------------------------------------------------------------------------------
    // Player 2
    //----------------------------------------------------------------------------------
    player2Acceleration = keyDetection(KEY_RIGHT, KEY_LEFT, KEY_UP,
                                       player2Acceleration, &isPlayer2Grounded);

    if (IsKeyPressed(KEY_P)) {
      debugMode = !debugMode;
    }

    // Apply gravity if not grounded
    if (!isPlayer2Grounded) {
      player2Acceleration.y += gravitationalForce;
    }

    // Update player2Velocity with player2Acceleration
    player2Velocity.x += player2Acceleration.x;
    player2Velocity.y += player2Acceleration.y;

    // Reset player2Acceleration
    player2Acceleration = (Vector2){0.0f, 0.0f};

    // Apply friction to player2Velocity
    player2Velocity.x *= friction;

    // Update position with player2Velocity
    if (player2Collider.x < 700 || player2Velocity.x < 0) {
      player2Collider.x += player2Velocity.x * dt;
    }
    player2Collider.y += player2Velocity.y * dt;

    player2Collider =
        IsGroundedDetection(player2Collider, backgroundCollider,
                            &player2Velocity.y, &isPlayer2Grounded);

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    if (!snakeRunning) {
      if (player2HP == 0 || player1HP == 0) {
        DrawText("GAME OVER", 30, screenHeight/2.5, 120,
                 RED); // Draw text (using default font)
      } else {
        ClearBackground(BLACK);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                               (Color){77, 180, 227, 100},
                               (Color){68, 121, 227, 100});
        DrawTexture(backgroundTexture, 0, screenHeight - 30, WHITE);

        if (debugMode) {
          DrawFPS(10, 10);
          printf("player1Collider(%f, %f) \n", player1Collider.x,
                 player1Collider.y);
          printf("player2Collider(%f, %f) \n", player2Collider.x,
                 player2Collider.y);
          printf("player1Velocity(%f, %f) \n", player1Velocity.x,
                 player1Velocity.y);
          printf("player2Velocity(%f, %f) \n", player2Velocity.x,
                 player2Velocity.y);
          printf("player1Acceleration(%f, %f) \n", player1Acceleration.x,
                 player1Acceleration.y);
          printf("player1Acceleration(%f, %f) \n", player1Acceleration.x,
                 player1Acceleration.y);
          printf("Difference: %f \n", player1Collider.x - player2Collider.x);
        }

        switch (playAnimation) {

        case 1:
          if (cooldownTimer1 >= 20) {
            if (player1AnimationTimer > 5) {
              player1FrameCounter++;
              player1AnimationTimer = 0;
            } else {
              player1AnimationTimer++;
            }

            if (player1FrameCounter < 11) {
              DrawTexturePro(player1Texture,
                             (Rectangle){SpriteSize * player1FrameCounter, 0,
                                         SpriteSize, SpriteSize},
                             (Rectangle){player1Collider.x, player1Collider.y,
                                         SpriteSize, SpriteSize},
                             (Vector2){0, 0}, -0.0f, WHITE);
            } else {
              playAnimation = 0;
              player1FrameCounter = 1;
              cooldownTimer1 = 0;
              if ((player1Collider.x - player2Collider.x) > -50) {
                player2HP -= 20;
                player2Acceleration.x += 100;
                player2Acceleration.y -= 30;
              }
            }
          } else {
            playAnimation = 0;
            break;
          }
        default:
          if (player1FrameCounter == 1) {
            DrawTexturePro(player1Texture,
                           (Rectangle){0, 0, SpriteSize, SpriteSize},
                           (Rectangle){player1Collider.x, player1Collider.y,
                                       SpriteSize, SpriteSize},
                           (Vector2){0, 0}, 0.0f, WHITE);
          }
          break;
        }

        switch (playAnimation2) {

        case 1:
          if (cooldownTimer2 >= 20) {
            if (player2AnimationTimer > 5) {
              player2FrameCounter++;
              player2AnimationTimer = 0;
            } else {
              player2AnimationTimer++;
            }

            if (player2FrameCounter < 11) {
              DrawTexturePro(player2Texture,
                             (Rectangle){SpriteSize * player2FrameCounter, 0,
                                         -SpriteSize, SpriteSize},
                             (Rectangle){player2Collider.x, player2Collider.y,
                                         SpriteSize, SpriteSize},
                             (Vector2){0, 0}, -0.0f, WHITE);
            } else {
              playAnimation2 = 0;
              player2FrameCounter = 1;
              cooldownTimer2 = 0;
              if ((player1Collider.x - player2Collider.x) > -50) {
                player1HP -= 20;
                player1Acceleration.x -= 100;
                player1Acceleration.y -= 30;
              }
            }
          } else {
            playAnimation2 = 0;
            break;
          }
        default:
          if (player2FrameCounter == 1) {
            DrawTexturePro(player2Texture,
                           (Rectangle){0, 0, -SpriteSize, SpriteSize},
                           (Rectangle){player2Collider.x, player2Collider.y,
                                       SpriteSize, SpriteSize},
                           (Vector2){0, 0}, -0.0f, WHITE);
          }
          break;
        }

        DrawRectangleLines(50, 50, 81, 11, GRAY);
        DrawRectangleGradientH(50, 50, player1HP - 20, 10,
                               (Color){255, 51, 65, 100}, RED);
        DrawRectangleLines(screenWidth - 50 - 80, 50, 81, 11, GRAY);
        DrawRectangleGradientH(screenWidth - 50 - 80, 50, player2HP - 20, 10,
                               (Color){255, 51, 65, 100}, RED);
      }
    }

    EndDrawing();
  }

  // De-Initialization
  UnloadTexture(backgroundTexture);
  UnloadTexture(player1Texture);
  UnloadTexture(player2Texture);
  UnloadImage(player1Image);
  UnloadImage(player2Image);
  CloseWindow();

  return 0;
}

//-------------------------------------------------------------------------------------

Rectangle IsGroundedDetection(Rectangle PlayerCollider,
                              Rectangle backgroundCollider, float *velocityY,
                              bool *isGrounded) {
  if (CheckCollisionRecs(PlayerCollider, backgroundCollider)) {
    PlayerCollider.y = backgroundCollider.y - PlayerCollider.height;
    *velocityY = 0;
    *isGrounded = true;
  } else {
    *isGrounded = false;
  }
  return PlayerCollider;
}

Vector2 keyDetection(KeyboardKey key1, KeyboardKey key2, KeyboardKey key3,
                     Vector2 accelerationVector, bool *isGrounded) {
  if (IsKeyDown(key1)) {
    accelerationVector.x += 2;
  }
  if (IsKeyDown(key2)) {
    accelerationVector.x -= 2;
  }
  if (IsKeyDown(key3) && *isGrounded) {
    accelerationVector.y -= 90.0f;
    *isGrounded = false;
  }
  return accelerationVector;
}
