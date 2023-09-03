#pragma once
#include "../Entity.h"
#include "../COMPONENTS.h"

class FallingBlock : public Entity {

	private:
		sf::Vector2f inital_position;

		bool simulated = false;
		bool triggered = false;
		float fall_buffer = 1.5f;

		float fall_buffer_tracked;

		Entity* image_entity;


	public:	
		sf::Vector2f dimensions;


		FallingBlock(sf::Vector2f position, sf::Vector2f dimensions);
		~FallingBlock();
		void awake() override;
		void reset();
		void trigger();
		void fall();

		void update() override;

		void on_collision_y(Collider*) override;
};
