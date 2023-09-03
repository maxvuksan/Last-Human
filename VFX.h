#pragma once
#include "COMPONENTS.h"

namespace VFX {
	class SurfaceImpact : public Entity {
		public:
		SurfaceImpact(int orientation=0)
		{
			auto* s1 = add_component<ParticleSource>(true);
			s1->colour = sf::Color(255, 255, 255);
			s1->lifetime = 2;
			s1->speed = 2.5;
			s1->speed_overtime = true;
			s1->size_overtime = true;
			s1->size = 2;
			s1->count = 3;
			s1->burst = true;
			s1->angle_min = 150 + orientation;
			s1->angle_max = 200 + orientation;

			auto* s2 = add_component<ParticleSource>(true);
			s2->colour = sf::Color(255, 255, 255);
			s2->lifetime = 2;
			s2->speed = 2.5;
			s2->speed_overtime = true;
			s2->size_overtime = true;
			s2->size = 2;
			s2->count = 3;
			s2->burst = true;
			s2->angle_min = -20 + orientation;
			s2->angle_max = 30 + orientation;

			remove_in(1);
		}
	};

}