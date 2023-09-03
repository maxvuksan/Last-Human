#pragma once
#include "../Entity.h"

class WeatherManager: public Entity {

	protected:
		float wind_speed = 0.1;
		float wind_strength = 8;
		float cycle_tracked;


	public:
		WeatherManager();
		void update() override;



};