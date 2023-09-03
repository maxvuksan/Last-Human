#include "Npc.h"
#include "../Game.h"
#include "Player.h"

void Npc::awake() {

	tag = "npc";
	range = add_component<RangeInteraction>(true);
	dialogue = add_component<DialogueSystem>(true);
	animator = add_component<Animator>(true);
	light = add_component<LightSource>(true);
	light->feather = 0.04; 
	light->drop_off = 0; 
	light->offset = sf::Vector2f(0, -4);

	animator->define_state("off", AnimationClip("Images/Tiles/computer.png", sf::Vector2f(16, 16), 0, 1));

	animator->define_state("on", AnimationClip("Images/Tiles/computer.png", sf::Vector2f(16, 16), 1, 3));
	animator->get_state("on").set_duration(90);

	rect.setSize(sf::Vector2f(8, 8));
}

void Npc::draw(sf::RenderTexture& surface) {


	Entity* target = game->find_entity_by_tag("player");
	auto player = dynamic_cast<Player*>(target);
	if (player != NULL) {
		player->interacting = dialogue->get_active();
	}

	if (dialogue->get_active()) {
		animator->change_state("on");
		light->set_active(true);
	}
	else {
		animator->change_state("off");
		light->set_active(false);
	}

	range->set_target(target);

}

void Npc::manage_events(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::X) {
			if (range->in_range && !dialogue->get_active()) {
				dialogue->start_dialogue();
			}
		}
		if (event.key.code == sf::Keyboard::C) {

			if (dialogue->get_active()) {
				dialogue->next_card();
			}

		}
	}
}

