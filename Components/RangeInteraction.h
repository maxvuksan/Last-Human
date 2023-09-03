#pragma once
#include "Component.h"


class RangeInteraction : public Component {

	private:
		float time_threshhold_tracked;

	protected:
		Entity* target = nullptr;
		float time_threshhold = 0.5; //must be in range for this long
		float distance = 40;

	public:
		bool in_range;

		RangeInteraction(Entity*);
		void update() override;
		void set_target(Entity*);



};
