#pragma once
#include "../Entity.h"
#include "../COMPONENTS.h"

class Spring : public Entity {

	protected:
		float bounce = 90;
		float delay = 2;
		float delay_tracked;

	public:

		void awake() override;
		void update() override;
		void on_trigger(Collider* other) override;
	
};