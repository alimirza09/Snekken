#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
Vector2 keyDetection(KeyboardKey key1, KeyboardKey key2, KeyboardKey key3,
                     Vector2 accelerationVector, bool *isGrounded) {
    if (IsKeyDown(key1)) {
        accelerationVector.x += 1;
    }
    if (IsKeyDown(key2)) {
        accelerationVector.x -= 1;
    }
    if (IsKeyDown(key3) && *isGrounded) {
        accelerationVector.y -= 90.0f;
        *isGrounded = false;
    }
    return accelerationVector;
}
Rectangle IsGroundedDetection(Rectangle PlayerCollider,Rectangle backgroundCollider,float *velocityY,bool *isGrounded){

        if (CheckCollisionRecs(PlayerCollider, backgroundCollider)) {
            PlayerCollider.y = backgroundCollider.y - PlayerCollider.height;
            *velocityY = 0;
            *isGrounded = true;
        } else {
            *isGrounded = false;
        }
        return PlayerCollider;
}
int main(void) {
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
    Rectangle backgroundCollider = {0, screenHeight - 30, backgroundTexture.width, backgroundTexture.height};

   // Player 1 
   bool isPlayer1Grounded = true;
    Image player1Image = LoadImage("assets/Enter-name.png");
    ImageResizeNN(&player1Image, 70, 130);
    Texture2D player1Texture = LoadTextureFromImage(player1Image);
    Rectangle player1Collider = {screenWidth / 2.0f - 25, backgroundCollider.y - player1Image.height, player1Image.width, player1Image.height};
    Vector2 velocityPlayer1 = {0.0f, 0.0f};
    Vector2 accelerationPlayer1 = {0.0f, 0.0f}; 

  // Player 2
    bool isPlayer2Grounded = true;
    Image player2Image = LoadImage("assets/Enter-name.png");
    ImageResizeNN(&player2Image, 70,130);
    ImageFlipHorizontal(&player2Image);
    Texture2D player2Texture = LoadTextureFromImage(player2Image);
  
    Rectangle player2Collider = {screenWidth / 2.0f - 25, backgroundCollider.y - player2Image.height,player2Image.width , player2Image.height};
    Vector2 velocityPlayer2 = {0.0f, 0.0f};
    Vector2 accelerationPlayer2 = {0.0f, 0.0f};


    // debugMode
    bool debugMode = true;

 // Friction coefficient
    float friction = 0.99; 

    // Main game loop
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        //----------------------------------------------------------------------------------
        // Player 1
        //----------------------------------------------------------------------------------
        accelerationPlayer1 = keyDetection(KEY_D, KEY_A, KEY_W, accelerationPlayer1, &isPlayer1Grounded);

        // Apply gravity if not grounded
        if (!isPlayer1Grounded) {
            accelerationPlayer1.y += 0.9;
        }

        // Update velocityPlayer1 with accelerationPlayer1
        velocityPlayer1.x += accelerationPlayer1.x;
        velocityPlayer1.y += accelerationPlayer1.y;

        // Reset accelerationPlayer1
        accelerationPlayer1 = (Vector2){0.0f, 0.0f};

        // Apply friction to the velocityPlayer1
        velocityPlayer1.x *= friction;

        // Update position with velocityPlayer1
        player1Collider.x += velocityPlayer1.x * dt;
        player1Collider.y += velocityPlayer1.y * dt;

        player1Collider = IsGroundedDetection(player1Collider,backgroundCollider,&velocityPlayer1.y,&isPlayer1Grounded);
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // Player 2
        //----------------------------------------------------------------------------------
        accelerationPlayer2 = keyDetection(KEY_RIGHT, KEY_LEFT, KEY_UP, accelerationPlayer2, &isPlayer2Grounded);

        if (IsKeyPressed(KEY_P)) {
            debugMode = !debugMode;
        }

        // Apply gravity if not grounded
        if (!isPlayer2Grounded) {
            accelerationPlayer2.y += 0.9;
        }

        // Update velocityPlayer2 with accelerationPlayer2
        velocityPlayer2.x += accelerationPlayer2.x;
        velocityPlayer2.y += accelerationPlayer2.y;

        // Reset accelerationPlayer2
        accelerationPlayer2 = (Vector2){0.0f, 0.0f};

        // Apply friction to the velocityPlayer2
        velocityPlayer2.x *= friction;

        // Update position with velocityPlayer2
        player2Collider.x += velocityPlayer2.x * dt;
        player2Collider.y += velocityPlayer2.y * dt;

        player2Collider = IsGroundedDetection(player2Collider,backgroundCollider,&velocityPlayer2.y,&isPlayer2Grounded);
        

        if (debugMode) {
            printf("velocityPlayer1 (%f, %f)\n", velocityPlayer1.x, velocityPlayer1.y);
        }

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(backgroundTexture, 0, screenHeight - 30, WHITE);

        if (debugMode) {
            DrawFPS(10, 10);
        }

       //   DrawRectangle(player1Collider.x, player1Collider.y, player1Collider.width, player1Collider.height, GOLD);
       //   DrawRectangle(player2Collider.x, player2Collider.y, player2Collider.width, player2Collider.height, RED);
      DrawTexture(player1Texture,player1Collider.x,player1Collider.y,WHITE);
      DrawTexture(player2Texture,player2Collider.x,player2Collider.y,WHITE);

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
