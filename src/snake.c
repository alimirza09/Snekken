#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

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

static bool pause = false;
static Food fruit = {0};
static int playerDied =
    0; // where 1 means player1 died, 2 means player2 died,3 means both players
       // died and 0 means game is in progress

static Snake snake1[SNAKE_LENGTH] = {0};
static Vector2 snake1Position[SNAKE_LENGTH] = {0};
static bool allowMoveSnake1 = false;
static Vector2 offsetSnake1 = {0};
static int counterTailSnake1 = 2;

static Snake snake2[SNAKE_LENGTH] = {0};
static Vector2 snake2Position[SNAKE_LENGTH] = {0};
static int counterTailSnake2 = 2;
static Vector2 offsetSnake2 = {0};
static bool allowMoveSnake2 = false;

void InitGame(void) {
  frameCounter = 0;
  counterTailSnake1 = 2;
  offsetSnake1.x = screenWidth % SQUARE_SIZE;
  offsetSnake1.y = screenHeight % SQUARE_SIZE;

  offsetSnake2.x = screenWidth % SQUARE_SIZE;
  offsetSnake2.y = screenHeight % SQUARE_SIZE;

  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake1[i].position = (Vector2){offsetSnake1.x / 2, offsetSnake1.y / 2};
    snake1[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake1[i].speed = (Vector2){SQUARE_SIZE, 0};

    if (i == 0)
      snake1[i].color = GREEN;

    else
      snake1[i].color = LIME;
  }
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake1Position[i] = (Vector2){0.0f, 0.0f};
  }

  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake2[i].position = (Vector2){offsetSnake2.x / 2, offsetSnake2.y / 2};
    snake2[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake2[i].speed = (Vector2){SQUARE_SIZE, 0};

    if (i == 0)
      snake2[i].color = BLUE;
    else
      snake2[i].color = SKYBLUE;
  }
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake1Position[i] = (Vector2){0.0f, 0.0f};
  }
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake2Position[i] = (Vector2){0.0f, 0.0f};
  }

  fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
  fruit.color = RED;
  fruit.active = false;
}

void UpdateGame(void) {
  if (playerDied != 3) {
    if (IsKeyPressed('P'))
      pause = !pause;

    if (!pause) {
      // Player control
      if (IsKeyPressed(KEY_D) && (snake1[0].speed.x == 0) && allowMoveSnake1) {
        snake1[0].speed = (Vector2){SQUARE_SIZE, 0};
        allowMoveSnake1 = false;
      }
      if (IsKeyPressed(KEY_A) && (snake1[0].speed.x == 0) && allowMoveSnake1) {
        snake1[0].speed = (Vector2){-SQUARE_SIZE, 0};
        allowMoveSnake1 = false;
      }
      if (IsKeyPressed(KEY_W) && (snake1[0].speed.y == 0) && allowMoveSnake1) {
        snake1[0].speed = (Vector2){0, -SQUARE_SIZE};
        allowMoveSnake1 = false;
      }
      if (IsKeyPressed(KEY_S) && (snake1[0].speed.y == 0) && allowMoveSnake1) {
        snake1[0].speed = (Vector2){0, SQUARE_SIZE};
        allowMoveSnake1 = false;
      }

      if (IsKeyPressed(KEY_RIGHT) && (snake2[0].speed.x == 0) &&
          allowMoveSnake2) {
        snake2[0].speed = (Vector2){SQUARE_SIZE, 0};
        allowMoveSnake2 = false;
      }
      if (IsKeyPressed(KEY_LEFT) && (snake2[0].speed.x == 0) &&
          allowMoveSnake2) {
        snake2[0].speed = (Vector2){-SQUARE_SIZE, 0};
        allowMoveSnake2 = false;
      }
      if (IsKeyPressed(KEY_UP) && (snake2[0].speed.y == 0) && allowMoveSnake2) {
        snake2[0].speed = (Vector2){0, -SQUARE_SIZE};
        allowMoveSnake2 = false;
      }
      if (IsKeyPressed(KEY_DOWN) && (snake2[0].speed.y == 0) &&
          allowMoveSnake2) {
        snake2[0].speed = (Vector2){0, SQUARE_SIZE};
        allowMoveSnake2 = false;
      }

      // snake1 movement
      for (int i = 0; i < counterTailSnake1; i++)
        snake1Position[i] = snake1[i].position;

      if ((frameCounter % 2) == 0) {
        for (int i = 0; i < counterTailSnake1; i++) {
          if (i == 0) {
            snake1[0].position.x += snake1[0].speed.x;
            snake1[0].position.y += snake1[0].speed.y;
            allowMoveSnake1 = true;
          } else
            snake1[i].position = snake1Position[i - 1];
        }
      }

      for (int i = 0; i < counterTailSnake2; i++)
        snake2Position[i] = snake2[i].position;

      if ((frameCounter % 2) == 0) {
        for (int i = 0; i < counterTailSnake2; i++) {
          if (i == 0) {
            snake2[0].position.x += snake2[0].speed.x;
            snake2[0].position.y += snake2[0].speed.y;
            allowMoveSnake2 = true;
          } else
            snake2[i].position = snake2Position[i - 1];
        }
      }

      // Wall behaviour
      if ((((snake1[0].position.x) > (screenWidth - offsetSnake1.x)) ||
           ((snake1[0].position.y) > (screenHeight - offsetSnake1.y)) ||
           (snake1[0].position.x < 0) || (snake1[0].position.y < 0)) &&
          (playerDied < 1)) {
        playerDied += 1;
      }
      if ((((snake2[0].position.x) > (screenWidth - offsetSnake2.x)) ||
           ((snake2[0].position.y) > (screenHeight - offsetSnake2.y)) ||
           (snake2[0].position.x < 0) || (snake2[0].position.y < 0)) &&
          (playerDied < 2)) {
        playerDied += 2;
      }

      // Collision with yourself
      for (int i = 1; i < counterTailSnake1; i++) {
        if ((snake1[0].position.x == snake1[i].position.x) &&
            (snake1[0].position.y == snake1[i].position.y))
          playerDied += 1;
      }

      for (int i = 1; i < counterTailSnake2; i++) {
        if ((snake2[0].position.x == snake2[i].position.x) &&
            (snake2[0].position.y == snake2[i].position.y))
          playerDied += 2;
      }

      // Fruit position calculation
      if (!fruit.active) {
        fruit.active = true;
        fruit.position = (Vector2){
            GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE +
                offsetSnake1.x / 2,
            GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE +
                offsetSnake1.y / 2};

        for (int i = 0; i < counterTailSnake1; i++) {
          while ((fruit.position.x == snake1[i].position.x) &&
                 (fruit.position.y == snake1[i].position.y)) {
            fruit.position =
                (Vector2){GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) *
                                  SQUARE_SIZE +
                              offsetSnake1.x / 2,
                          GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) *
                                  SQUARE_SIZE +
                              offsetSnake1.y / 2};
            i = 0;
          }
        }
      }

      // Collision
      if ((snake1[0].position.x < (fruit.position.x + fruit.size.x) &&
           (snake1[0].position.x + snake1[0].size.x) > fruit.position.x) &&
          (snake1[0].position.y < (fruit.position.y + fruit.size.y) &&
           (snake1[0].position.y + snake1[0].size.y) > fruit.position.y)) {
        snake1[counterTailSnake1].position =
            snake1Position[counterTailSnake1 - 1];
        counterTailSnake1 += 1;
        fruit.active = false;
      }

      if ((snake2[0].position.x < (fruit.position.x + fruit.size.x) &&
           (snake2[0].position.x + snake2[0].size.x) > fruit.position.x) &&
          (snake2[0].position.y < (fruit.position.y + fruit.size.y) &&
           (snake2[0].position.y + snake2[0].size.y) > fruit.position.y)) {
        snake2[counterTailSnake2].position =
            snake2Position[counterTailSnake2 - 1];
        counterTailSnake2 += 1;
        fruit.active = false;
      }

      frameCounter++;
    }
  } else {
    if (IsKeyPressed(KEY_ENTER)) {
      InitGame();
      playerDied = 0;
    }
  }
}

// Draw game (one frame)
void DrawGame(void) {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  if (playerDied != 3) {
    // Draw grid lines
    for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++) {
      DrawLineV(
          (Vector2){SQUARE_SIZE * i + offsetSnake1.x / 2, offsetSnake1.y / 2},
          (Vector2){SQUARE_SIZE * i + offsetSnake1.x / 2,
                    screenHeight - offsetSnake1.y / 2},
          LIGHTGRAY);
    }

    for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++) {
      DrawLineV(
          (Vector2){offsetSnake1.x / 2, SQUARE_SIZE * i + offsetSnake1.y / 2},
          (Vector2){screenWidth - offsetSnake1.x / 2,
                    SQUARE_SIZE * i + offsetSnake1.y / 2},
          LIGHTGRAY);
    }

    // Draw snake1
    if (playerDied != 1)
      for (int i = 0; i < counterTailSnake1; i++)
        DrawRectangleV(snake1[i].position, snake1[i].size, snake1[i].color);

    if (playerDied != 2)
      for (int i = 0; i < counterTailSnake2; i++)
        DrawRectangleV(snake2[i].position, snake2[i].size, snake2[i].color);

    // Draw fruit to pick
    DrawRectangleV(fruit.position, fruit.size, fruit.color);
    char buff[50]; // Common buffer

    sprintf(buff, "player 1 score: %d", counterTailSnake1 - 2);
    DrawText(buff, 5, 5, 20, RED);

    sprintf(buff, "player 2 score:  %d", counterTailSnake2 - 2);
    DrawText(buff, screenWidth - 5 - 200, 5, 20, RED);

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

// Update and Draw (one frame)
Vector2 UpdateDrawFrame(void) {
  UpdateGame();
  DrawGame();
  if (playerDied == 3) {
    return (Vector2){counterTailSnake1 - 2, counterTailSnake2 - 2};
  }
  return (Vector2){256, 256};
}
// int main() {
//   SetTargetFPS(60);
//   InitWindow(screenWidth, screenHeight, "snake");
//   InitGame();
//   while (!WindowShouldClose()) {
//     printf("%d \n", playerDied);
//     UpdateDrawFrame();
//   }
// }
