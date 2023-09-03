#include "DialogueSystem.h"
#include "../Game.h"

DialogueSystem::DialogueSystem(Entity* entity):Component(entity)
{
	set_render_layer(99);
	this->active = false;
	cards.push_back(DialogueCard("welcome to monolith city,"));
	cards.push_back(DialogueCard("current residency: 28,000"));

	rectangle_shape.setPosition(sf::Vector2f(edge_buffer, edge_buffer));
	rectangle_shape.setFillColor(rectangle_colour);
	rectangle_shape.setSize(sf::Vector2f(entity->game->get_display_size().x - 2 * edge_buffer, box_height));
	rectangle_shape.setOutlineColor(sf::Color(255, 226, 184));
	rectangle_shape.setOutlineThickness(1);

	text.setFont(entity->game->font);
	text.setPosition(sf::Vector2f(edge_buffer * 2, edge_buffer * 2));
	text.setCharacterSize(5);
	text.setFillColor(sf::Color(255, 226, 184)); //250, 192, 105
}


void DialogueSystem::draw(sf::RenderTexture& surface) {

	if (!this->active) {
		return;
	}
	 
	if (type_speed_tracked < type_speed) {
		type_speed_tracked++;
	}
	else {
		if (current_character < cards[current_card].content.size()) {
			type_speed_tracked = 0;
			current_character++;
			entity->game->play_sound("dialogue.wav");
		}
	}


	text.setString(cards[current_card].content.substr(0, current_character));
	surface.draw(rectangle_shape);
	surface.draw(text);
}

void DialogueSystem::next_card() {
	current_card++;
	current_character = 0;
	type_speed_tracked = 0;
	if (current_card >= cards.size()) {
		this->active = false;
	}
	current_card %= cards.size();
}

void DialogueSystem::start_dialogue() {
	current_card = 0;
	this->active = true;
}

