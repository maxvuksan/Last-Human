#pragma once
#include "../Entity.h"

class Lamp : public Entity {

	private:
		sf::Sprite sprite;
		sf::Texture texture;

	public:
		Lamp();
		void awake() override;
		void draw(sf::RenderTexture&) override;
};
