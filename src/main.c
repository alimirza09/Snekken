#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
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
  // Vector2 rectanglePosition = {(float)screenWidth / 2, (float)screenHeight / 2};
  Image background = LoadImage("assets/background.png");
  ImageResizeNN(&background, 800, 30);
  Texture2D backgroundTexture = LoadTextureFromImage(background); // Image converted to texture, uploaded to GPU memory (VRAM)
  UnloadImage(background);                               // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
  Rectangle testRec = {400 - 50, 225 - 50, 50, 50};// Rectangle backgroundCollider = {0, 420, backgroundTexture.width, backgroundTexture.height};
  Rectangle backgroundCollider={0, 420, 800,30};
  Vector2 velocity = {(float)0, (float)0};
  bool debugMode = true;
  Vector2 acceleration = {(float)0, (float)0};
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_D))
    {
      velocity.x += 10.0;
    }
    if (IsKeyDown(KEY_A))
    {
      acceleration.x -= 10.0;
    }
    if (IsKeyDown(KEY_W) && CheckCollisionRecs(backgroundCollider, testRec))
    {
      acceleration.y -= 10.0;
    }
    if (IsKeyDown(KEY_P))
    {
      if(debugMode){
        debugMode = false;
      }
      else{
        debugMode = true;
      }
    }
    // Friction
    if (velocity.x < 0)
    {
      velocity.x += 5;
    }
    if (velocity.x > 0)
    {
      velocity.x -= 5;
    }
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
    // Gravity
    if (testRec.y != backgroundCollider.y)
    {
      acceleration.y += 10;
    }
    testRec.y += velocity.y * dt;
    testRec.x += velocity.x * dt;
    if (debugMode){
      printf("Rectangle (%f, %f)", testRec.x, testRec.y);
    }
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(backgroundTexture, backgroundCollider.x, backgroundCollider.y, WHITE);
    DrawFPS(10, 10);
    DrawRectangle(testRec.x, testRec.y, testRec.width, testRec.height, GOLD);
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
