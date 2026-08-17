#pragma once 

#include <vector>
#include <cmath>

namespace nbody_stars {

    struct Stars {
        std::vector<float> x, y, z;
        std::vector<float> velocity_x, velocity_y, velocity_z;
        std::vector<float> mass;

        size_t count() const { return x.size(); }

        void add(float px, float py, float pz, float m) {
            x.push_back(px); y.push_back(py); z.push_back(pz);
            velocity_x.push_back(0); velocity_y.push_back(0); velocity_z.push_back(0);
            mass.push_back(m);
        }

        inline float CalculateRadius(float mass) {
            return std::cbrt(mass) * 0.3f;
        }
    };

} // namespace nbody_stars