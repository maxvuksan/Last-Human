#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Particle.h"

class ParticleSource: public Component {

	protected:
		std::vector<Particle*> particles;

		float delay_tracked = 0;
		bool burst_tracked = false;

	public:
		bool follow_parent = true;
		bool speed_overtime = true;
		bool size_overtime = true;

		bool burst = false;

		double speed = 1;
		float angle_min = 0;
		float angle_max = 360;
		float angle_overtime = 0;
		int count = 1;
		int size = 5;
		int lifetime = 20;
		int delay = 15;
		int outline_thickness = 0;

		sf::Color colour = sf::Color(255,255,255);
		sf::Color outline_colour = sf::Color(0,0,0);

		ParticleSource(Entity*);
		~ParticleSource();

		void add_particle(Particle* particle);
		void remove_particle(Particle* particle);

		void draw(sf::RenderTexture&);
};

