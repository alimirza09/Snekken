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
  Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};
  Image background = LoadImage("assets/background.png");
  Texture2D texture = LoadTextureFromImage(background); // Image converted to texture, uploaded to GPU memory (VRAM)
  UnloadImage(background);                              // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

  //--------------------------------------------------------------------------------------
  int FPS;
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_D))
      ballPosition.x += 200.0f * dt;
    if (IsKeyDown(KEY_A))
      ballPosition.x -= 200.0f * dt;
    if (IsKeyDown(KEY_W))
      ballPosition.y -= 200.0f * dt;
    if (IsKeyDown(KEY_S))
      ballPosition.y += 200.0f * dt;
    printf("(%f, %f)", ballPosition.x, ballPosition.y);

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(texture, 750, 350, WHITE);

    DrawFPS(10, 10);
    DrawCircleV(ballPosition, 50, MAROON);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //-------------------------------------------------------------------------------------
  UnloadTexture(texture);
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
