#include "raylib.h"
#include "stdio.h"
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

  Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    int Fps = GetFPS();
    char char_fps[4]; 
    snprintf(char_fps, 4, "%d", Fps);
    float speed = 120.0f;
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_D)) ballPosition.x += (speed*dt);
    if (IsKeyDown(KEY_A)) ballPosition.x -= (speed*dt);
    if (IsKeyDown(KEY_W)) ballPosition.y -= (speed*dt);
    if (IsKeyDown(KEY_S)) ballPosition.y += (speed*dt);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(char_fps, 10, 10, 20, DARKGRAY);

    DrawCircleV(ballPosition, 50, MAROON);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
