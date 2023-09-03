#pragma once
#include "../Entity.h"

class Npc : public Entity {

	protected:
		RangeInteraction* range;
		DialogueSystem* dialogue;
		Animator* animator;
		LightSource* light;

		sf::RectangleShape rect;
		

	public:
		void awake() override;
		void draw(sf::RenderTexture&) override;
		void manage_events(sf::Event) override;

};