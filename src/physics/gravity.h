#pragma once

#include "objects/stars.h"

namespace nbody_stars {

	struct SimulationParameters {
		float delta_time;
		float gravitational_constant;
		float softening_factor;
	};

	void UpdateGravity(Stars& stars, const SimulationParameters& params);

} // namespace nbody_stars