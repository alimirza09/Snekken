#include <raylib.h>
#include <stdio.h>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Snekken");
  ChangeDirectory("..");
  Vector2 rectanglePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
  Image background = LoadImage("assets/Enter-name.png");
  ImageResizeNN(&background, 800, 150);
  Texture2D backgroundTexture = LoadTextureFromImage(background); // Image converted to texture, uploaded to GPU memory (VRAM)
  UnloadImage(background);                                        // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

  //--------------------------------------------------------------------------------------
  int FPS;
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    struct Rectangle testRec = {50, 50, rectanglePosition.x, rectanglePosition.y};
    struct Rectangle backgroundCollider = {0, 300, backgroundTexture.width, backgroundTexture.height};
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_D))
      rectanglePosition.x += 100.0f * dt;
    if (IsKeyDown(KEY_A))
      rectanglePosition.x -= 100.0f * dt;
    if (IsKeyDown(KEY_W))
      rectanglePosition.y -= 95.0f * dt;
    if (IsKeyDown(KEY_S) && !CheckCollisionRecs(backgroundCollider, testRec))
      rectanglePosition.y += 100.0f * dt;
    testRec.x = rectanglePosition.x;
    testRec.y = rectanglePosition.y;
    if (!CheckCollisionRecs(backgroundCollider, testRec))
    {
      rectanglePosition.y += 5;
    }
    printf("Rectangle (%f, %f)", rectanglePosition.x, rectanglePosition.y);
    printf("Background(%d, %d)", backgroundTexture.height, backgroundTexture.width);
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(backgroundTexture, 0, 300, WHITE);
    DrawFPS(10, 10);
    DrawRectangle(rectanglePosition.x, rectanglePosition.y, 50, 50, GOLD);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //-------------------------------------------------------------------------------------
  UnloadTexture(backgroundTexture);
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
