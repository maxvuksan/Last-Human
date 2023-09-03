#include "FallingBlock.h"
#include "../Game.h"
#include "Player.h"
#include <math.h>
#include <random>

FallingBlock::FallingBlock(sf::Vector2f position, sf::Vector2f dimensions) {
	tag = "falling_block";
	inital_position = position;
	this->dimensions = dimensions;
}

void FallingBlock::awake() {
	image_entity = new Entity;
	game->add_entity(image_entity);
	image_entity->add_component<Tilemap>();
	image_entity->get_component<Tilemap>()->set_cellsize(game->cellsize);
	image_entity->get_component<Tilemap>()->regenerate(dimensions);
	image_entity->get_component<Tilemap>()->set_all_tiles(1);
	image_entity->transform->set_parent(this->transform);

	add_component<BoxCollider>(true)->set_dimensions(dimensions * (float)game->cellsize);
	get_component<BoxCollider>()->is_centered(false);

	reset();
}

void FallingBlock::reset() {
	if (get_component<PhysicsBody>() != nullptr) {
		get_component<PhysicsBody>()->set_active(false);
		get_component<PhysicsBody>()->restart_gravity();
	}
	simulated = false;
	triggered = false;
	fall_buffer_tracked = 0;
	transform->set_position(inital_position);
}

void FallingBlock::on_collision_y(Collider* col) {

	if (col->get_entity()->tag == "player" && !triggered) {

		auto player = dynamic_cast<Player*>(col->get_entity());

		if (player->grounded_by(get_component<Collider>())) {
			trigger();
		}
	}
}

void FallingBlock::update() {
	if (triggered && !simulated) {
		if (fall_buffer_tracked <= 0) {
			fall();
		}
		else {

			float x = sin((rand() % 2) - 1);
			float y = cos((rand() % 2) - 1);

			image_entity->transform->set_local_position(sf::Vector2f(x, y));

			fall_buffer_tracked -= game->delta_time;
		}
	}
}

void FallingBlock::fall() {
	if (!simulated) {
		image_entity->transform->set_local_position(sf::Vector2f(0, 0));
		if (get_component<PhysicsBody>() == nullptr) {
			add_component<PhysicsBody>();
			get_component<PhysicsBody>()->set_gravity(3);
			get_component<PhysicsBody>()->set_constraint_x(true);
		}
		else {
			get_component<PhysicsBody>()->set_active(true);
			get_component<PhysicsBody>()->set_velocity_y(0);
		}
		simulated = true;
	}
}

void FallingBlock::trigger() {
	if (!triggered) {
		fall_buffer_tracked = fall_buffer;
		triggered = true;
	}
}

FallingBlock::~FallingBlock() {
	game->remove_entity(image_entity);
}