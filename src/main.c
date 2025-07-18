#include "raylib.h"
#include "snake.c"
// #include "snake.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define SPRITE_SIZE 36 * 4

typedef struct {
  int animationTimer;
  int frameCounter;
  int animation;
  int cooldownTimer;
  Texture2D texture;
  Rectangle collider;
  Vector2 acceleration;
  Vector2 velocity;
  float hp;
  int mana;
  bool isGrounded;
} Player;

Vector2 keyDetection(KeyboardKey leftKey, KeyboardKey rightKey,
                     KeyboardKey jumpKey, KeyboardKey punchKey,
                     KeyboardKey kickKey, KeyboardKey specialKey,
                     Player *player);

Rectangle IsGroundedDetection(Rectangle PlayerCollider,
                              Rectangle backgroundCollider, float *velocityY,
                              bool *isGrounded);

Vector2 UpdatePosition(Vector2 accelerationVector, Vector2 velocityVector,
                       Rectangle *Collider, Rectangle Collider2, float friction,
                       float dt, int playerNumber);

bool IsCooldownReady(int cooldownTimer, int requiredTime);

void DrawPlayerAnimation(Player *player, int frameRow, bool flip);

void UpdatePlayerAnimation(Player *player, int maxFrames, int frameRow,
                           int frameInterval, bool flip);

void HandlePlayerAnimation(Player *player, Player *opponent, int frameInterval,
                           int maxFrames, int cooldown, int hpReduction,
                           int manaIncrease, int xAccel, int yAccel,
                           int frameRow, bool flip);

Texture2D SafeLoadTexture(const char *filename);

Image SafeLoadImage(const char *filename);

int main() {
  //----------------------------------------------------------------------------------
  // Initialization
  //----------------------------------------------------------------------------------

  ChangeDirectory(".."); // assets not found if you dont do this

  InitWindow(screenWidth, screenHeight, "Snekken");
  InitGame();
  Image icon = LoadImage("assets/ICON.png");
  SetWindowIcon(icon);

  Image laserBeam = LoadImage("assets/EnterNameReworked.png");

  ImageCrop(&laserBeam, (Rectangle){36 * 16, 36 * 3, 36, 36});

  Image background = LoadImage("assets/background.png");
  ImageResizeNN(&background, screenWidth, 30);
  Texture2D backgroundTexture = LoadTextureFromImage(background);
  UnloadImage(background);
  Rectangle backgroundCollider = {0, screenHeight - 30, backgroundTexture.width,
                                  backgroundTexture.height};

  //----------------------------------------------------------------------------------
  // Player 1
  //----------------------------------------------------------------------------------
  Texture2D player1Texture = SafeLoadTexture("assets/EnterNameReworked.png");
  Rectangle player1Collider = {screenWidth / 2.0f - SPRITE_SIZE,
                               backgroundCollider.y - SPRITE_SIZE - 2,
                               SPRITE_SIZE - 48, SPRITE_SIZE};
  double multiplier;
  Player player1 = {0,
                    0,
                    0,
                    100,
                    player1Texture,
                    player1Collider,
                    {0.0f, 0.0f},
                    {0.0f, 0.0f},
                    100,
                    0,
                    true};

  //----------------------------------------------------------------------------------
  // Player 2
  //----------------------------------------------------------------------------------
  Texture2D player2Texture = SafeLoadTexture("assets/EnterNameReworked.png");
  Rectangle player2Collider = {screenWidth / 2.0f + 25,
                               backgroundCollider.y - SPRITE_SIZE - 2,
                               SPRITE_SIZE - 36, SPRITE_SIZE};
  Player player2 = {0,               // animationTimer
                    0,               // frameCounter
                    0,               // animation
                    100,             // cooldownTimer
                    player2Texture,  // texture
                    player2Collider, // collider
                    {0.0f, 0.0f},    // acceleration
                    {0.0f, 0.0f},    // velocity
                    100,             // hp
                    0,               // mana
                    true};           // isGrounded

  bool debugMode = false;
  bool minigameMode = false;

  const float gravitationalForce = 4;
  // Friction coefficient
  const float friction = 0.95;

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    if (minigameMode) {
      Vector2 playerScore = UpdateDrawFrame();
      if ((playerScore.x != 256) && (playerScore.y != 256)) {
        if (playerScore.y == 0) {
          multiplier = playerScore.x;
        } else {
          multiplier = playerScore.x / playerScore.y;
        }
        player1.animationTimer += 1;
        minigameMode = false;
        InitGame();
        playerDied = 0;
      }
    } else {
      float dt = GetFrameTime();
      player2.cooldownTimer += 150 * dt;
      player1.cooldownTimer += 150 * dt;
      if (IsKeyPressed(KEY_R)) {
        player1Collider = (Rectangle){screenWidth / 2.0f - SPRITE_SIZE,
                                      backgroundCollider.y - SPRITE_SIZE - 2,
                                      SPRITE_SIZE - 48, SPRITE_SIZE};

        player2Collider = (Rectangle){screenWidth / 2.0f + 25,
                                      backgroundCollider.y - SPRITE_SIZE - 2,
                                      SPRITE_SIZE - 36, SPRITE_SIZE};

        player1 = (Player){0,               // animationTimer;
                           0,               // frameCounter;
                           0,               // animation;
                           100,             // cooldownTimer;
                           player1Texture,  // texture;
                           player1Collider, // collider;
                           {0.0f, 0.0f},    // acceleration
                           {0.0f, 0.0f},    // velocity;
                           100,             // hp;
                           0,               // mana;
                           true};           // isGrounded;

        player2 = (Player){0,
                           0,
                           0,
                           100,
                           player2Texture,
                           player2Collider,
                           {0.0f, 0.0f},
                           {0.0f, 0.0f},
                           100,
                           0,
                           true};
      }
      //----------------------------------------------------------------------------------
      // Player 1
      //----------------------------------------------------------------------------------
      if (player1.mana > 100) {
        player1.mana = 100;
      }
      player1.acceleration =
          keyDetection(KEY_D, KEY_A, KEY_W, KEY_C, KEY_X, KEY_V, &player1);

      // Apply gravity if not grounded
      if (!player1.isGrounded) {
        player1.acceleration.y += gravitationalForce;
      }
      player1.velocity =
          UpdatePosition(player1.acceleration, player1.velocity,
                         &player1.collider, player2.collider, friction, dt, 1);
      player1.acceleration = (Vector2){0, 0};
      player1.collider =
          IsGroundedDetection(player1.collider, backgroundCollider,
                              &player1.velocity.y, &player1.isGrounded);

      //----------------------------------------------------------------------------------
      // Player 2
      //----------------------------------------------------------------------------------
      if (player2.mana > 100) {
        player2.mana = 100;
      }

      player2.acceleration = keyDetection(KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_J,
                                          KEY_K, KEY_L, &player2);

      if (IsKeyPressed(KEY_P)) {
        debugMode = !debugMode;
      }

      // Apply gravity if not grounded
      if (!player2.isGrounded) {
        player2.acceleration.y += gravitationalForce;
      }

      player2.velocity =
          UpdatePosition(player2.acceleration, player2.velocity,
                         &player2.collider, player1.collider, friction, dt, 2);
      player2.acceleration = (Vector2){0, 0};

      player2.collider =
          IsGroundedDetection(player2.collider, backgroundCollider,
                              &player2.velocity.y, &player2.isGrounded);

      if (debugMode) {
        DrawFPS(10, 10);
        printf("player1.collider(%f, %f) \n", player1.collider.x,
               player1.collider.y);
        printf("player2.collider(%f, %f) \n", player2.collider.x,
               player2.collider.y);
        printf("player1.velocity(%f, %f) \n", player1.velocity.x,
               player1.velocity.y);
        printf("player2.velocity(%f, %f) \n", player2.velocity.x,
               player2.velocity.y);
        printf("player1.acceleration(%f, %f) \n", player1.acceleration.x,
               player1.acceleration.y);
        printf("player1.acceleration(%f, %f) \n", player1.acceleration.x,
               player1.acceleration.y);
        printf("player1.animation: %d \n", player1.animation);
        printf("player2.animation: %d \n", player2.animation);
        printf("player1.frameCounter: %d \n", player1.frameCounter);
        printf("player2.frameCounter: %d \n", player2.frameCounter);
        printf("dt: %f \n", dt);
        printf("player1.hp: %f \n", player1.hp);
        printf("player2.hp: %f \n", player2.hp);
        printf("player1.isGrounded: %d \n", player1.isGrounded);
      }

      //----------------------------------------------------------------------------------
      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();
      if (minigameMode == true) {
        minigameMode = true;
      } else if (player2.hp <= 0) {
        DrawText("PLAYER 1 WINS", 28, screenHeight / 2.5, 90, RED);
      } else if (player1.hp <= 0) {
        DrawText("PLAYER 2 WINS", 28, screenHeight / 2.5, 90, RED);
      } else {
        ClearBackground(BLACK);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                               (Color){77, 180, 227, 100},
                               (Color){68, 121, 227, 100});
        DrawTexture(backgroundTexture, 0, screenHeight - 30, WHITE);

        // Player1 animation handling
        switch (player1.animation) {
        case 1:
          HandlePlayerAnimation(&player1, &player2, 2, 12, 20, 10, 30, 100, -90,
                                0, false);
          break;

        case 2:
          HandlePlayerAnimation(&player1, &player2, 5, 7, 55, 15, 10, 150, -90,
                                1, false);
          break;

        case 3:
          HandlePlayerAnimation(&player1, &player2, 6, 7, 0, 0, 0, 0, 0, 2,
                                false);
          break;

        case 4:
          if (player1.mana < 100) {
            printf("insufficient mana \n");
            player1.animation = 0;
            player1.frameCounter = 0;
            player1.animationTimer = 0;
            DrawPlayerAnimation(&player1, 0, false);
            break;
          }
          DrawPlayerAnimation(&player1, 3, false);
          if (player1.frameCounter < 15) {
            if (player1.animationTimer > 1) {
              player1.frameCounter++;
              player1.animationTimer = 0;
            } else {
              player1.animationTimer++;
            }
          } else if (player1.frameCounter == 15) {
            if (player1.animationTimer > 100) {
              player1.animation = 0;
              player1.frameCounter = 0;
              player1.animationTimer = 0;
              player1.mana = 0;
              DrawPlayerAnimation(&player1, 0, false);
            }
            if (player1.animationTimer == 0) {
              minigameMode = true;
            } else {
              ImageResizeNN(&laserBeam,
                            player2.collider.x - player1.collider.x + 1,
                            SPRITE_SIZE);
              Texture2D laserBeamTexture = LoadTextureFromImage(laserBeam);
              DrawTexturePro(
                  laserBeamTexture,
                  (Rectangle){0, 0, player2.collider.x - player1.collider.x,
                              SPRITE_SIZE},
                  (Rectangle){
                      player1.collider.x + SPRITE_SIZE, player1.collider.y,
                      player2.collider.x - player1.collider.x - SPRITE_SIZE,
                      SPRITE_SIZE},
                  (Vector2){0, 0}, 0.0f, WHITE);

              player1.animationTimer++;
              player2.hp -= 0.5 * multiplier;
            }
          }
          break;
        default:
          DrawPlayerAnimation(&player1, 0, false);
          break;
        }

        // Player2 animation handling
        switch (player2.animation) {
        case 1:
          HandlePlayerAnimation(&player2, &player1, 2, 12, 10, 20, 30, -100,
                                -30, 0, true);
          break;
        case 2:
          HandlePlayerAnimation(&player2, &player1, 5, 7, 55, 15, 10, -150, -90,
                                1, true);
          break;
        case 3:
          HandlePlayerAnimation(&player2, &player1, 6, 7, 0, 0, 0, 0, 0, 2,
                                true);
          break;
        case 4:
          if (player2.mana < 100) {
            player2.animation = 0;
            DrawPlayerAnimation(&player2, 0, true);
          } else {
            if ((player2Collider.x - player1Collider.x - 48) <= 0) {
              printf("\033[31;1;4m%f\033[0m \n",
                     player2Collider.x - player1Collider.x - 48);
              break;
            } else {
              ImageResizeNN(&laserBeam,
                            player2Collider.x - player1Collider.x - 48,
                            SPRITE_SIZE);
            }
            Texture2D laserBeamTexture = LoadTextureFromImage(laserBeam);
            DrawTexture(laserBeamTexture,
                        player2.collider.x - laserBeamTexture.width,
                        player2.collider.y, WHITE);
            UnloadTexture(laserBeamTexture);
          }

        default:
          DrawPlayerAnimation(&player2, 0, true);
          break;
        }

        // HP Draw
        DrawRectangleLines(50, 50, 81, 11, GRAY);
        DrawRectangleLines(screenWidth - 50 - 80, 50, 100 / 1.22, 11, GRAY);
        DrawRectangleGradientH(50, 50, player1.hp / 1.249, 10,
                               (Color){255, 51, 65, 100}, RED);
        DrawRectangleGradientH(screenWidth - 50 - 80, 50, player2.hp / 1.249,
                               10, (Color){255, 51, 65, 100}, RED);
        // Mana Draw
        DrawRectangleLines(50, 50 + 11, 81, 5, GRAY);
        DrawRectangleLines(screenWidth - 50 - 80, 50 + 11, 100 / 1.22, 5, GRAY);

        DrawRectangleGradientH(50, 50 + 11, player1.mana / 1.249, 4,
                               (Color){57, 8, 255, 100}, BLUE);
        DrawRectangleGradientH(screenWidth - 50 - 80, 50 + 11,
                               player2.mana / 1.249, 4,
                               (Color){57, 8, 255, 100}, BLUE);

        // Player Identifier draw
        DrawText("PLAYER 1", 50, 40, 5, RAYWHITE);
        DrawText("PLAYER 2", screenWidth - 50 - 80, 40, 5, RAYWHITE);
        // Colliders draw
        if (debugMode) {
          DrawRectangleLines(player1.collider.x, player1.collider.y,
                             player1.collider.width, player1.collider.height,
                             RAYWHITE);
          DrawRectangleLines(player2.collider.x, player2.collider.y,
                             player2.collider.width, player2.collider.height,
                             RAYWHITE);
        }
      }

      EndDrawing();
    }
  }

  // De-Initialization
  UnloadTexture(backgroundTexture);
  UnloadTexture(player1.texture);
  UnloadTexture(player2.texture);
  CloseWindow();

  return 0;
}

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
Vector2 keyDetection(KeyboardKey leftKey, KeyboardKey rightKey,
                     KeyboardKey jumpKey, KeyboardKey punchKey,
                     KeyboardKey kickKey, KeyboardKey specialKey,
                     Player *player) {
  if (IsKeyDown(leftKey)) {
    player->acceleration.x += 3;
  }
  if (IsKeyDown(rightKey)) {
    player->acceleration.x -= 3;
  }
  if (IsKeyDown(jumpKey) && player->isGrounded && player->animation != 3) {
    player->animation = 3;
    player->frameCounter = 0;
    player->animationTimer = 0;
  }
  if (IsKeyPressed(punchKey) && player->animation != 1) {
    player->animation = 1;
    player->frameCounter = 0;
    player->animationTimer = 0;
  }
  if (IsKeyPressed(kickKey) && player->animation != 2) {
    player->animation = 2;
    player->frameCounter = 0;
    player->animationTimer = 0;
  }
  if (IsKeyPressed(specialKey) && player->animation != 4) {
    player->animation = 4;
    player->frameCounter = 0;
    player->animationTimer = 0;
  }
  return player->acceleration;
}
Vector2 UpdatePosition(Vector2 accelerationVector, Vector2 velocityVector,
                       Rectangle *Collider, Rectangle Collider2, float friction,
                       float dt, int playerNumber) {

  // Update velocityVector with accelerationVector
  velocityVector.x += accelerationVector.x;
  velocityVector.y += accelerationVector.y;

  // Apply friction to velocityVector
  velocityVector.x *= friction;

  if (playerNumber == 1) {
    if ((!CheckCollisionRecs(*Collider, Collider2) || velocityVector.x <= 0) &&
        (Collider->x > -40 || velocityVector.x >= 0)) {
      Collider->x += velocityVector.x * dt;
    } else {
      velocityVector.x = 0;
    }
  } else {
    if ((!CheckCollisionRecs(*Collider, Collider2) || velocityVector.x >= 0) &&
        (Collider->x < screenWidth - Collider->width + 52 ||
         velocityVector.x <= 0)) {
      Collider->x += velocityVector.x * dt;
    } else {
      velocityVector.x = 0;
    }
  }
  Collider->y += velocityVector.y * dt;
  return velocityVector;
}

// Function to check cooldown
bool IsCooldownReady(int cooldownTimer, int requiredTime) {
  return cooldownTimer >= requiredTime;
}

// Function to draw player animations
void DrawPlayerAnimation(Player *player, int frameRow, bool flip) {
  int flipModifier = flip ? -1 : 1;
  DrawTexturePro(player->texture,
                 (Rectangle){SPRITE_SIZE * player->frameCounter,
                             frameRow * SPRITE_SIZE, flipModifier * SPRITE_SIZE,
                             SPRITE_SIZE},
                 (Rectangle){player->collider.x + (flip ? -48 : 0),
                             player->collider.y, SPRITE_SIZE, SPRITE_SIZE},
                 (Vector2){0, 0}, 0.0f, WHITE);
}

void UpdatePlayerAnimation(Player *player, int maxFrames, int frameRow,
                           int frameInterval, bool flip) {
  if (player->animationTimer > frameInterval && frameRow < 2) {
    player->frameCounter++;
    player->animationTimer = 0;
  } else if (player->animationTimer > frameInterval && frameRow == 2) {
    if (player->frameCounter < 3 || player->frameCounter >= 5) {
      player->frameCounter++;
      player->animationTimer = 0;
    } else if ((player->frameCounter <= 6 && player->frameCounter > 3) &&
               player->isGrounded) {
      player->frameCounter++;
      player->animationTimer = 0;
    } else if (player->frameCounter == 3) {
      player->acceleration.y -= 100.0f;
      player->isGrounded = false;
      player->frameCounter++;
    }
  } else {
    player->animationTimer++;
  }

  if (player->frameCounter <= maxFrames) {
    int flipModifier = flip ? -1 : 1;
    DrawPlayerAnimation(player, frameRow, flip);
  }

  if (player->frameCounter >= maxFrames) {
    player->animation = 0;
    player->frameCounter = 0;
    player->cooldownTimer = 0;
  }
}

void HandlePlayerAnimation(Player *player, Player *opponent, int frameInterval,
                           int maxFrames, int cooldown, int hpReduction,
                           int manaIncrease, int xAccel, int yAccel,
                           int frameRow, bool flip) {
  if (IsCooldownReady(player->cooldownTimer, cooldown)) {

    UpdatePlayerAnimation(player, maxFrames, frameRow, frameInterval, flip);

    if (player->animation == 0) {
      if ((player->collider.x - opponent->collider.x) > -120 &&
          (player->collider.x - opponent->collider.x) < 120) {
        if (opponent->animation < 3 && opponent->animation != 0) {
          opponent->hp -= hpReduction + 10;
          player->mana += manaIncrease + 20;
          printf("COUNTER\n");
          opponent->animation = 0;
          opponent->frameCounter = 0;
          opponent->animationTimer = 0;
        } else {
          opponent->hp -= hpReduction;
          player->mana += manaIncrease;
        }
        opponent->acceleration.x += xAccel;
        opponent->acceleration.y += yAccel;
      }
    }
  } else {
    player->animation = 0;
  }
}
Image SafeLoadImage(const char *filename) {
  if (FileExists(filename)) {
    Image image = LoadImage(filename);
    return image;
  } else {
    printf("\033[31;1;4mError: Failed to load texture: %s\033[0m"
           "\n",
           filename);
    return (Image){0};
  }
}

Texture2D SafeLoadTexture(const char *filename) {
  if (FileExists(filename)) {
    Image image = LoadImage(filename);
    ImageResizeNN(&image, SPRITE_SIZE * 17, SPRITE_SIZE * 5);
    if (image.data != NULL) {
      Texture2D texture = LoadTextureFromImage(image);
      UnloadImage(image);
      return texture;
    }
  }
  printf("\033[31;1;4mError: Failed to load texture: %s\033[0m"
         "\n",
         filename);
  return (Texture2D){0}; // Return a null/invalid texture
}
