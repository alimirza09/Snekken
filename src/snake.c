#include "raylib.h"
#include <stdbool.h>

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 15

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
  int foodValue;
} Food;

static const int screenWidth = 800;
static const int screenHeight = 450;

static int frameCounter = 0;

static bool gameOver = false;
static bool pause = false;
static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static bool allowMove = false;
static Vector2 offset = {0};
static int counterTail = 2;

void InitGame(void) {
  frameCounter = 0;
  counterTail = 2;
  offset.x = screenWidth % SQUARE_SIZE;
  offset.y = screenHeight % SQUARE_SIZE;

  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake[i].position = (Vector2){offset.x / 2, offset.y / 2};
    snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake[i].speed = (Vector2){SQUARE_SIZE, 0};

    if (i == 0)
      snake[i].color = GREEN;

    else
      snake[i].color = LIME;
  }
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snakePosition[i] = (Vector2){0.0f, 0.0f};
  }

  fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
  fruit.color = RED;
  fruit.active = false;
}

void UpdateGame(void) {
  if (!gameOver) {
    if (IsKeyPressed('P'))
      pause = !pause;

    if (!pause) {
      // Player control
      if (IsKeyPressed(KEY_D) && (snake[0].speed.x == 0) && allowMove) {
        snake[0].speed = (Vector2){SQUARE_SIZE, 0};
        allowMove = false;
      }
      if (IsKeyPressed(KEY_A) && (snake[0].speed.x == 0) && allowMove) {
        snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
        allowMove = false;
      }
      if (IsKeyPressed(KEY_W) && (snake[0].speed.y == 0) && allowMove) {
        snake[0].speed = (Vector2){0, -SQUARE_SIZE};
        allowMove = false;
      }
      if (IsKeyPressed(KEY_S) && (snake[0].speed.y == 0) && allowMove) {
        snake[0].speed = (Vector2){0, SQUARE_SIZE};
        allowMove = false;
      }

      // Snake movement
      for (int i = 0; i < counterTail; i++)
        snakePosition[i] = snake[i].position;

      if ((frameCounter % 10) == 0) {
        for (int i = 0; i < counterTail; i++) {
          if (i == 0) {
            snake[0].position.x += snake[0].speed.x;
            snake[0].position.y += snake[0].speed.y;
            allowMove = true;
          } else
            snake[i].position = snakePosition[i - 1];
        }
      }

      // Wall behaviour
      if (((snake[0].position.x) > (screenWidth - offset.x)) ||
          ((snake[0].position.y) > (screenHeight - offset.y)) ||
          (snake[0].position.x < 0) || (snake[0].position.y < 0)) {
        gameOver = true;
      }

      // Collision with yourself
      for (int i = 1; i < counterTail; i++) {
        if ((snake[0].position.x == snake[i].position.x) &&
            (snake[0].position.y == snake[i].position.y))
          gameOver = true;
      }

      // Fruit position calculation
      if (!fruit.active) {
        fruit.active = true;
        fruit.position = (Vector2){
            GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE +
                offset.x / 2,
            GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE +
                offset.y / 2};

        for (int i = 0; i < counterTail; i++) {
          while ((fruit.position.x == snake[i].position.x) &&
                 (fruit.position.y == snake[i].position.y)) {
            fruit.position =
                (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) *
                                  SQUARE_SIZE +
                              offset.x / 2,
                          GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) *
                                  SQUARE_SIZE +
                              offset.y / 2};
            i = 0;
          }
        }
      }

      // Collision
      if ((snake[0].position.x < (fruit.position.x + fruit.size.x) &&
           (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
          (snake[0].position.y < (fruit.position.y + fruit.size.y) &&
           (snake[0].position.y + snake[0].size.y) > fruit.position.y)) {
        snake[counterTail].position = snakePosition[counterTail - 1];
        counterTail += 1;
        fruit.active = false;
      }

      frameCounter++;
    }
  } else {
    if (IsKeyPressed(KEY_ENTER)) {
      InitGame();
      gameOver = false;
    }
  }
}

// Draw game (one frame)
void DrawGame(void) {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  if (!gameOver) {
    // Draw grid lines
    for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++) {
      DrawLineV((Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2},
                (Vector2){SQUARE_SIZE * i + offset.x / 2,
                          screenHeight - offset.y / 2},
                LIGHTGRAY);
    }

    for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++) {
      DrawLineV(
          (Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2},
          (Vector2){screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2},
          LIGHTGRAY);
    }

    // Draw snake
    for (int i = 0; i < counterTail; i++)
      DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

    // Draw fruit to pick
    DrawRectangleV(fruit.position, fruit.size, fruit.color);

    if (pause)
      DrawText("GAME PAUSED",
               screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2,
               screenHeight / 2 - 40, 40, GRAY);
  } else
    DrawText("PRESS [ENTER] TO PLAY AGAIN",
             GetScreenWidth() / 2 -
                 MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
             GetScreenHeight() / 2 - 50, 20, GRAY);

  EndDrawing();
}

// Unload game variables
void UnloadGame(void) {
  // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void) {
  UpdateGame();
  DrawGame();
}
// int main() {
//   SetTargetFPS(60);
//   InitWindow(screenWidth, screenHeight, "snake");
//   InitGame();
//   while (!WindowShouldClose()) {
//     UpdateDrawFrame();
//   }
//   UnloadGame();
// }
