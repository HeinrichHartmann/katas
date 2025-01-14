#include <raylib.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    printf("Starting application...\n");
    
    // Initialize window with basic settings
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    printf("Initializing window...\n");
    InitWindow(screenWidth, screenHeight, "PhotoSort");
    
    if (!IsWindowReady()) {
        printf("Failed to initialize window\n");
        return 1;
    }
    printf("Window initialized successfully\n");
    
    // Basic window state
    SetTargetFPS(60);
    
    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        // Simple static text, no animations yet
        DrawText("Hello World!", 
                screenWidth/2 - 100,
                screenHeight/2 - 30,
                60,
                BLACK);
        
        EndDrawing();
    }
    
    printf("Closing window...\n");
    CloseWindow();
    printf("Window closed\n");
    
    return 0;
}
