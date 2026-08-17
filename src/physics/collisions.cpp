#include "physics/collisions.h"
#include <cmath>
#include <vector>

namespace nbody_stars {

    void ResolveCollisions(Stars& stars) {
        const size_t star_count = stars.count();
        std::vector<bool> is_absorbed(star_count, false);

        for (size_t i = 0; i < star_count; i++) {
            if (is_absorbed[i]) continue;

            for (size_t j = i + 1; j < star_count; j++) {
                if (is_absorbed[j]) continue;

                const float delta_x = stars.x[j] - stars.x[i];
                const float delta_y = stars.y[j] - stars.y[i];
                const float delta_z = stars.z[j] - stars.z[i];
                const float distance_squared = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;

                const float combined_radius = stars.CalculateRadius(stars.mass[i]) + stars.CalculateRadius(stars.mass[j]);

                if (distance_squared > combined_radius * combined_radius) {
                    continue;
                }

                const float total_mass = stars.mass[i] + stars.mass[j];

                const float merged_position_x = (stars.x[i] * stars.mass[i] + stars.x[j] * stars.mass[j]) / total_mass;
                const float merged_position_y = (stars.y[i] * stars.mass[i] + stars.y[j] * stars.mass[j]) / total_mass;
                const float merged_position_z = (stars.z[i] * stars.mass[i] + stars.z[j] * stars.mass[j]) / total_mass;

                const float merged_velocity_x =
                    (stars.velocity_x[i] * stars.mass[i] + stars.velocity_x[j] * stars.mass[j]) / total_mass;
                const float merged_velocity_y =
                    (stars.velocity_y[i] * stars.mass[i] + stars.velocity_y[j] * stars.mass[j]) / total_mass;
                const float merged_velocity_z =
                    (stars.velocity_z[i] * stars.mass[i] + stars.velocity_z[j] * stars.mass[j]) / total_mass;

                stars.x[i] = merged_position_x;
                stars.y[i] = merged_position_y;
                stars.z[i] = merged_position_z;
                stars.velocity_x[i] = merged_velocity_x;
                stars.velocity_y[i] = merged_velocity_y;
                stars.velocity_z[i] = merged_velocity_z;
                stars.mass[i] = total_mass;

                is_absorbed[j] = true;
            }
        }

        size_t write_index = 0;
        for (size_t read_index = 0; read_index < star_count; read_index++) {
            if (is_absorbed[read_index]) continue;

            if (write_index != read_index) {
                stars.x[write_index] = stars.x[read_index];
                stars.y[write_index] = stars.y[read_index];
                stars.z[write_index] = stars.z[read_index];
                stars.velocity_x[write_index] = stars.velocity_x[read_index];
                stars.velocity_y[write_index] = stars.velocity_y[read_index];
                stars.velocity_z[write_index] = stars.velocity_z[read_index];
                stars.mass[write_index] = stars.mass[read_index];
            }
            write_index++;
        }

        stars.x.resize(write_index);
        stars.y.resize(write_index);
        stars.z.resize(write_index);
        stars.velocity_x.resize(write_index);
        stars.velocity_y.resize(write_index);
        stars.velocity_z.resize(write_index);
        stars.mass.resize(write_index);
    }

} // namespace nbody_stars