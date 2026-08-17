#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "N-body Stars Simulation - by MrEffect");

    SetTargetFPS(0);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("N-body Stars Project Initialized!", 190, 280, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}