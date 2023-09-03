#include "Spring.h"
#include "../Game.h"

void Spring::awake() {

	tag = "spring";
	render_layer = 1;
	BoxCollider* b = add_component<BoxCollider>(true);
	b->set_dimensions(sf::Vector2f(10, 4));
	b->is_trigger = true;
	b->offset = sf::Vector2f(0, 6);

	Animator* a = add_component<Animator>(true);
	a->define_state("idle", AnimationClip("Images/Tiles/spring.png", sf::Vector2f(16, 16), 0, 1));
	a->define_state("bounce", AnimationClip("Images/Tiles/spring.png", sf::Vector2f(16, 16), 1, 4, false));
	a->get_state("bounce").set_duration({ 100, 60, 25 });

}

void Spring::update() {
	if (delay_tracked > 0) {
		delay_tracked -= game->delta_time;
	}
}

void Spring::on_trigger(Collider* other) {
	if (delay_tracked <= 0) {
		if (other->get_entity()->tag == "player") {
			delay_tracked = delay;
			game->play_sound("spring1.wav", 3);

			other->get_entity()->get_component<PhysicsBody>()->set_velocity_y(-bounce);
			other->get_entity()->get_component<PhysicsBody>()->restart_gravity();
			get_component<Animator>()->change_state("bounce");
		}
	}
}