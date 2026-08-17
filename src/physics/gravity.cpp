#include "gravity.h"
#include <cmath>

namespace nbody_stars {

    void UpdateGravity(Stars& stars, const SimulationParameters& params)
	{
		const size_t star_count = stars.count();
		const float softening_squared = params.softening_factor * params.softening_factor;

		std::vector<float> force_x(star_count, 0.0f);
		std::vector<float> force_y(star_count, 0.0f);
		std::vector<float> force_z(star_count, 0.0f);

        const float* __restrict position_x = stars.x.data();
        const float* __restrict position_y = stars.y.data();
        const float* __restrict position_z = stars.z.data();
        const float* __restrict mass = stars.mass.data();

        float* __restrict out_force_x = force_x.data();
        float* __restrict out_force_y = force_y.data();
        float* __restrict out_force_z = force_z.data();

        for (size_t i = 0; i < star_count; i++) {
            const float current_x = position_x[i];
            const float current_y = position_y[i];
            const float current_z = position_z[i];
            const float current_mass = mass[i];

            float accumulated_force_x = 0.0f;
            float accumulated_force_y = 0.0f;
            float accumulated_force_z = 0.0f;

            for (size_t j = 0; j < star_count; j++) {
                const float delta_x = position_x[j] - current_x;
                const float delta_y = position_y[j] - current_y;
                const float delta_z = position_z[j] - current_z;

                const float distance_squared =
                    delta_x * delta_x + delta_y * delta_y + delta_z * delta_z + softening_squared;
                const float inverse_distance = 1.0f / std::sqrt(distance_squared);
                const float inverse_distance_cubed =
                    inverse_distance * inverse_distance * inverse_distance;

                const float force_magnitude =
                    params.gravitational_constant * current_mass * mass[j] * inverse_distance_cubed;

                accumulated_force_x += force_magnitude * delta_x;
                accumulated_force_y += force_magnitude * delta_y;
                accumulated_force_z += force_magnitude * delta_z;
            }

            out_force_x[i] = accumulated_force_x;
            out_force_y[i] = accumulated_force_y;
            out_force_z[i] = accumulated_force_z;
        }

        for (size_t i = 0; i < star_count; i++) {
            const float inverse_mass = 1.0f / stars.mass[i];
            stars.velocity_x[i] += force_x[i] * inverse_mass * params.delta_time;
            stars.velocity_y[i] += force_y[i] * inverse_mass * params.delta_time;
            stars.velocity_z[i] += force_z[i] * inverse_mass * params.delta_time;
        }

        for (size_t i = 0; i < star_count; i++) {
            stars.x[i] += stars.velocity_x[i] * params.delta_time;
            stars.y[i] += stars.velocity_y[i] * params.delta_time;
            stars.z[i] += stars.velocity_z[i] * params.delta_time;
        }
	}

} // namespace nbody_stars