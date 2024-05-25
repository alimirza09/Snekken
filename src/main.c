#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  //----------------------------------------------------------------------------------
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Snekken");
  ChangeDirectory("..");
  Image background = LoadImage("assets/background.png");
  ImageResizeNN(&background, 800, 30);
  Texture2D backgroundTexture = LoadTextureFromImage(background);
  UnloadImage(background);

  bool isGrounded = true;
  Rectangle backgroundCollider = {0, 420, backgroundTexture.width,
                                  backgroundTexture.height};
  Rectangle testRec = {screenWidth / 2 - 50, backgroundCollider.y - 50, 50, 50};
  Vector2 velocity = {0.0f, 0.0f};
  bool debugMode = true;
  Vector2 acceleration = {0.0f, 0.0f};

  // Main game loop
  while (!WindowShouldClose()) {
    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    float dt = GetFrameTime();

    // Reset acceleration
    acceleration = (Vector2){0.0f, 0.0f};

    if (IsKeyDown(KEY_D)) {
      acceleration.x += 1;
    }
    if (IsKeyDown(KEY_A)) {
      acceleration.x -= 1;
    }
    if (IsKeyDown(KEY_W) && isGrounded) {
      acceleration.y -= 90.0f;
      isGrounded = false;
    }
    if (IsKeyPressed(KEY_P)) {
      debugMode = !debugMode;
    }

    // Apply gravity if not grounded
    if (!isGrounded) {
      acceleration.y += 0.6;
    }

    // Update velocity with acceleration
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    // Apply friction to the velocity
    float friction = 0.99;  // Friction coefficient
    velocity.x *= friction;

    // Update position with velocity
    testRec.x += velocity.x * dt;
    testRec.y += velocity.y * dt;

    // Check for collision with ground
    if (CheckCollisionRecs(testRec, backgroundCollider)) {
      testRec.y = backgroundCollider.y - testRec.height;
      velocity.y = 0;
      isGrounded = true;
    } else {
      isGrounded = false;
    }

    if (debugMode) {
      printf("Velocity (%f, %f)\n", velocity.x, velocity.y);
    }

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(backgroundTexture, backgroundCollider.x, backgroundCollider.y,
                WHITE);

    if (debugMode) {
      DrawFPS(10, 10);
    }

    DrawRectangle(testRec.x, testRec.y, testRec.width, testRec.height, GOLD);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //-------------------------------------------------------------------------------------
  UnloadTexture(backgroundTexture);
  CloseWindow();
  //--------------------------------------------------------------------------------------

  return 0;
}
