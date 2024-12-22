#include <raylib.h>
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
  int hp;
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
                           int xAccel, int yAccel, int frameRow, bool flip);

Texture2D SafeLoadTexture(const char *filename);

const int screenWidth = 800;
const int screenHeight = 450;

int main() {
  //----------------------------------------------------------------------------------
  // Initialization
  //----------------------------------------------------------------------------------

  // Load Icon Image
  ChangeDirectory("..");

  InitWindow(screenWidth, screenHeight, "Snekken");
  Image icon = LoadImage("assets/ICON.png");
  SetWindowIcon(icon);

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
  Texture2D player1Texture = SafeLoadTexture("assets/EnterNameReworked.png");
  Rectangle player1Collider = {screenWidth / 2.0f - SPRITE_SIZE,
                               backgroundCollider.y - SPRITE_SIZE - 2,
                               SPRITE_SIZE - 48, SPRITE_SIZE};
  Player player1 = {0,
                    0,
                    0,
                    100,
                    player1Texture,
                    player1Collider,
                    {0.0f, 0.0f},
                    {0.0f, 0.0f},
                    100,
                    true};

  //----------------------------------------------------------------------------------
  // Player 2
  //----------------------------------------------------------------------------------
  Texture2D player2Texture = SafeLoadTexture("assets/EnterNameReworked.png");
  Rectangle player2Collider = {screenWidth / 2.0f + 25,
                               backgroundCollider.y - SPRITE_SIZE - 2,
                               SPRITE_SIZE - 36, SPRITE_SIZE};
  Player player2 = {0,
                    0,
                    0,
                    100,
                    player2Texture,
                    player2Collider,
                    {0.0f, 0.0f},
                    {0.0f, 0.0f},
                    100,
                    true};

  // Debug mode and snake mode
  bool debugMode = false;

  const float gravitationalForce = 4;

  // Friction coefficient
  const float friction = 0.99;

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
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
      player1 = (Player){0,
                         0,
                         0,
                         100,
                         player1Texture,
                         player1Collider,
                         {0.0f, 0.0f},
                         {0.0f, 0.0f},
                         100,
                         true};
      player2 = (Player){0,
                         0,
                         0,
                         100,
                         player2Texture,
                         player2Collider,
                         {0.0f, 0.0f},
                         {0.0f, 0.0f},
                         100,
                         true};
    }
    //----------------------------------------------------------------------------------
    // Player 1
    //----------------------------------------------------------------------------------
    player1.acceleration =
        keyDetection(KEY_D, KEY_A, KEY_W, KEY_X, KEY_C, KEY_V, &player1);

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
    player2.acceleration = keyDetection(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_J,
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

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    if (player2.hp <= 0) {
      DrawText("PLAYER 1 WINS", 28, screenHeight / 2.5, 90, RED);
    } else if (player1.hp <= 0) {
      DrawText("PLAYER 2 WINS", 28, screenHeight / 2.5, 90, RED);
    } else {

      ClearBackground(BLACK);
      DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                             (Color){77, 180, 227, 100},
                             (Color){68, 121, 227, 100});
      DrawTexture(backgroundTexture, 0, screenHeight - 30, WHITE);

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
        printf("player1.animationTimer: %d \n", player1.animationTimer);
        printf("player2.animationTimer: %d \n", player2.animationTimer);
        printf("player1.frameCounter: %d \n", player1.frameCounter);
        printf("player2.frameCounter: %d \n", player2.frameCounter);
        printf("dt: %f \n", dt);
      }

      switch (player1.animation) {
      case 1:
        HandlePlayerAnimation(&player1, &player2, 2, 12, 20, 20, 100, -90, 0,
                              false);
        break;
      case 2:
        HandlePlayerAnimation(&player1, &player2, 5, 7, 55, 35, 150, -90, 1,
                              false);
        break;
      case 3:
        HandlePlayerAnimation(&player1, &player2, 6, 7, 0, 0, 0, 0, 2, false);
        break;
      case 4:
        if (IsKeyDown(KEY_V)) {
          if (player1.frameCounter < 16) {
            if (player1.animationTimer > 5) {
              player1.frameCounter++;
              player1.animationTimer = 0;
            } else {
              player1.animationTimer++;
            }
          // TODO: MAKE FRAME 16 prolonged
          } else if (player1.frameCounter == 16) {
            if (player1.animationTimer < 1000) {
              player1.animationTimer++;
              player1.frameCounter = 16;
          DrawPlayerAnimation(&player1, 3, false);
            } else {
              player1.animation = 0;
              player1.frameCounter = 0;
              player1.animationTimer = 0;
            }
          }
          DrawPlayerAnimation(&player1, 3, false);

        } else {
          player1.animation = 0;
          player1.frameCounter = 0;
          player1.animationTimer = 0;
          DrawPlayerAnimation(&player1, 0, false);
        }
        break;
      default:
        DrawPlayerAnimation(&player1, 0, false);
        break;
      }

      switch (player2.animation) {
      case 1:
        HandlePlayerAnimation(&player2, &player1, 2, 12, 20, 20, -100, -30, 0,
                              true);
        break;
      case 2:
        HandlePlayerAnimation(&player2, &player1, 5, 7, 55, 35, -150, -90, 1,
                              true);
        break;
      case 3:
        HandlePlayerAnimation(&player2, &player1, 6, 7, 0, 0, 0, 0, 2, true);
        break;
      case 4:
        HandlePlayerAnimation(&player2, &player1, 4, 16, 2000, 50, -150, -90, 3,
                              true);
        break;
      default:
        DrawPlayerAnimation(&player2, 0, true);
        break;
      }

      // HP Draw
      DrawRectangleLines(50, 50, 81, 11, GRAY);
      DrawRectangleLines(screenWidth - 50 - 80, 50, 100 / 1.22, 11, GRAY);
      DrawRectangleGradientH(50, 50, player1.hp / 1.249, 10,
                             (Color){255, 51, 65, 100}, RED);
      DrawRectangleGradientH(screenWidth - 50 - 80, 50, player2.hp / 1.249, 10,
                             (Color){255, 51, 65, 100}, RED);

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
    player->acceleration.x += 2;
  }
  if (IsKeyDown(rightKey)) {
    player->acceleration.x -= 2;
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
                           int xAccel, int yAccel, int frameRow, bool flip) {
  if (IsCooldownReady(player->cooldownTimer, cooldown)) {

    UpdatePlayerAnimation(player, maxFrames, frameRow, frameInterval, flip);

    if (player->animation == 0) {
      if ((player->collider.x - opponent->collider.x) > -120 &&
          (player->collider.x - opponent->collider.x) < 120) {
        if (opponent->animation > 3) {
          opponent->hp -= hpReduction + 10;
        } else {
          opponent->hp -= hpReduction;
        }
        opponent->acceleration.x += xAccel;
        opponent->acceleration.y += yAccel;
      }
    }
  } else {
    player->animation = 0;
  }
}

Texture2D SafeLoadTexture(const char *filename) {
  if (FileExists(filename)) {
    Image image = LoadImage(filename);
    ImageResizeNN(&image, SPRITE_SIZE * 16, SPRITE_SIZE * 5);
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
