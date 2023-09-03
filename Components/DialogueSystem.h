#pragma once
#include "Component.h"
#include "DialogueCard.h"

class DialogueSystem : public Component {



	protected:
		sf::RectangleShape rectangle_shape;
		std::vector<DialogueCard> cards;
		sf::Text text;

		sf::Color rectangle_colour = sf::Color(25, 20, 40, 255);

		int current_character;
		short current_card;
		float type_speed_tracked;

		int edge_buffer = 4;
		int box_height = 40;
		float type_speed = 2.2f;

	public:
		DialogueSystem(Entity*);

		void draw(sf::RenderTexture&) override;

		void next_card();
		void start_dialogue();
};