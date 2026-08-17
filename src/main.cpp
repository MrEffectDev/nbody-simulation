#include <cmath>
#include <random>

#include "raylib.h"
#include "objects/stars.h"
#include "physics/gravity.h"
#include "physics/collisions.h"


int main() {
    const int screen_width = 800;
    const int screen_height = 600;
    InitWindow(screen_width, screen_height, "N-body Stars Simulation - by MrEffect");
    SetTargetFPS(0);

    nbody_stars::Stars stars;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist_x(0.0f, static_cast<float>(screen_width));
    std::uniform_real_distribution<float> dist_y(0.0f, static_cast<float>(screen_height));
    std::uniform_real_distribution<float> dist_z(0.0f, static_cast<float>(screen_height));
    std::uniform_real_distribution<float> dist_mass(10.0f, 100.0f);

    for (size_t i = 0; i < 512; i++) {
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

    while (!WindowShouldClose()) {
        params.delta_time = GetFrameTime() * 10;
        nbody_stars::UpdateGravity(stars, params);
        nbody_stars::ResolveCollisions(stars);

        BeginDrawing();
        ClearBackground(BLACK);
        for (size_t i = 0; i < stars.count(); i++) {
            const float radius = std::cbrt(stars.mass[i]) * 0.3f;
            DrawCircleV({ stars.x[i], stars.y[i] }, radius, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}