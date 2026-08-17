#include <cmath>
#include <random>
#include "raylib.h"
#include "objects/stars.h"
#include "physics/gravity.h"
#include "physics/collisions.h"

enum class RenderMode {
    Mode2D,
    Mode3D,
};

int main() {
    const int screen_width = 800;
    const int screen_height = 600;
    InitWindow(screen_width, screen_height, "N-body Stars Simulation - by MrEffect");
    SetTargetFPS(0);

    nbody_stars::Stars stars;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist_x(-200.0f, 200.0f);
    std::uniform_real_distribution<float> dist_y(-200.0f, 200.0f);
    std::uniform_real_distribution<float> dist_z(-200.0f, 200.0f);
    std::uniform_real_distribution<float> dist_mass(10.0f, 100.0f);

    for (size_t i = 0; i < 1024; i++) {
        float px = dist_x(gen);
        float py = dist_y(gen);
        float pz = dist_z(gen);
        float mass = dist_mass(gen);
        stars.add(px, py, pz, mass);
    }

    nbody_stars::SimulationParameters params{
        .delta_time = 0.0f,
        .gravitational_constant = 0.0667f,
        .softening_factor = 5.0f,
    };

    Camera3D camera{};
    camera.position = { 0.0f, 200.0f, 500.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Camera2D camera_2d{};
    camera_2d.offset = { screen_width / 2.0f, screen_height / 2.0f };
    camera_2d.target = { 0.0f, 0.0f };
    camera_2d.zoom = 1.0f;

    RenderMode render_mode = RenderMode::Mode2D;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) {
            render_mode = (render_mode == RenderMode::Mode2D) ? RenderMode::Mode3D : RenderMode::Mode2D;
            if (render_mode == RenderMode::Mode3D) {
                DisableCursor();
            }
            else {
                EnableCursor();
            }
        }

        params.delta_time = GetFrameTime() * 10;
        nbody_stars::UpdateGravity(stars, params);
        nbody_stars::ResolveCollisions(stars);

        if (render_mode == RenderMode::Mode3D) {
            Vector3 movement = { 0.0f, 0.0f, 0.0f };
            if (IsKeyDown(KEY_W)) movement.x += 1.0f;
            if (IsKeyDown(KEY_S)) movement.x -= 1.0f;
            if (IsKeyDown(KEY_D)) movement.y += 1.0f;
            if (IsKeyDown(KEY_A)) movement.y -= 1.0f;
            if (IsKeyDown(KEY_SPACE)) movement.z += 1.0f;
            if (IsKeyDown(KEY_LEFT_SHIFT)) movement.z -= 1.0f;

            Vector2 mouse_delta = GetMouseDelta();
            Vector3 rotation = { mouse_delta.x * 0.05f, mouse_delta.y * 0.05f, 0.0f };

            UpdateCameraPro(&camera, movement, rotation, GetMouseWheelMove() * 2.0f);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (render_mode == RenderMode::Mode2D) {
            BeginMode2D(camera_2d);
            for (size_t i = 0; i < stars.count(); i++) {
                const float radius = std::cbrt(stars.mass[i]) * 0.3f;
                DrawCircleV({ stars.x[i], stars.y[i] }, radius, RAYWHITE);
            }
            EndMode2D();
        }
        else {
            BeginMode3D(camera);
            for (size_t i = 0; i < stars.count(); i++) {
                const float radius = std::cbrt(stars.mass[i]) * 0.3f;
                const Vector3 position = { stars.x[i], stars.y[i], stars.z[i] };
                DrawSphere(position, radius, RAYWHITE);
            }
            EndMode3D();
        }

        DrawText(render_mode == RenderMode::Mode2D ? "2D  [TAB]" : "3D  [TAB]", 10, 10, 20, GRAY);
        DrawText("WASD - Move", 10, screen_height - 35, 10, GRAY);
        DrawText("SHIFT/SPACE - UP/DOWN", 10, screen_height - 20, 10, GRAY);
        DrawText(TextFormat("FPS: %d", GetFPS()), screen_width - 100, 10, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}