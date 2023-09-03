#include "BoxCollider.h"
#include "../Entity.h"
#include "../Game.h"
#include "PhysicsBody.h"

BoxCollider::BoxCollider(Entity* entity) : Collider(entity) {
	Collider::awake();

	this->dimensions = dimensions;
	this->center = true;
	offset = sf::Vector2f(0, 0);
}

void BoxCollider::update() {
	
	debug_shape.setOutlineColor(sf::Color(0, 255, 0));

	if (is_trigger) {

		triggered = false;
		colliders_in_trigger = check_for_collisions();
		if (colliders_in_trigger.size() > 0) {
			triggered = true;
		}

		debug_shape.setOutlineColor(sf::Color(255, 255, 0));
		if (triggered) {
			debug_shape.setOutlineColor(sf::Color(255, 100, 0));
		}
	}
}

void BoxCollider::draw(sf::RenderTexture& surface) {

	if (entity->game->debug_mode) {
		debug_shape.setSize(dimensions);
		debug_shape.setPosition(entity->transform->get_draw_position() + offset);
		if (center) {
			debug_shape.setOrigin(sf::Vector2f(dimensions.x / 2, dimensions.y / 2));
		}
		debug_shape.setFillColor(sf::Color(0, 0, 0, 0));
		
		debug_shape.setOutlineThickness(1);

		surface.draw(debug_shape);
	}

}

void BoxCollider::set_dimensions(sf::Vector2f dimensions){
	this->dimensions = dimensions;
}
sf::Vector2f const BoxCollider::get_dimensions() {
	return dimensions;
}
void BoxCollider::is_centered(bool state) {
	center = state;
}
bool BoxCollider::is_centered() {
	return center;
}

bool BoxCollider::overlapping(Collider& other) {

	if ((other.right() > left() && other.left() < right())){

		if ((other.top() < bottom() && other.bottom() > top())) {
			return true;
		}
	}
	return false;
}

bool BoxCollider::point_in_collider(sf::Vector2f position) {
	if ((position.x >= left() && position.x <= right())) {

		if ((position.y <= bottom() && position.y >= top())) {
			return true;
		}
	}
	return false;
}
bool BoxCollider::point_in_collider(sf::Vector2i position) {
	return point_in_collider(sf::Vector2f(position.x, position.y));
}

std::vector<Collider*> BoxCollider::check_for_collisions() {
	
	std::vector<Collider*> overlapped_colliders;

	//iterates through all the colliders
	for (int i = 0; i < entity->game->get_colliders().size(); i++) {

		//checks each collider if it overlaps with any colliders
		Collider* current_col = entity->game->get_colliders()[i];

		if (this->get_entity() != (*current_col).get_entity() && overlapping(*current_col)) {

			int layer1 = entity->collision_layer;
			int layer2 = current_col->get_entity()->collision_layer;

			if (get_entity()->game->collision_matrix_check(layer1, layer2)) {

				if (!current_col->is_trigger)
				{ // the collider is not a trigger
					overlapped_colliders.push_back(current_col);

					if (is_trigger) { //is a trigger
						current_col->get_entity()->in_trigger(this);
						entity->on_trigger(current_col);
					}
				}

			}
		}
	}
	return overlapped_colliders;
}

void BoxCollider::move(sf::Vector2f movement, bool constraint_x, bool constraint_y) {

	sf::Vector2f new_position = entity->transform->get_position() + movement;
	if (!is_trigger) {
		new_position = entity->transform->get_position();
		bool col_x_found = false, col_y_found = false;

		PhysicsBody* phys = entity->get_component<PhysicsBody>();

		float friction = 0;
		if (!constraint_x) {
			//collision adjustment is done one axis at a time
			entity->transform->translate(sf::Vector2f(movement.x, 0));
			auto collisions_x = check_for_collisions();

			for (int i = 0; i < collisions_x.size(); i++) {
				if (movement.x > 0) {
					//hitting collider from the left side
					if (collisions_x[i]->collision_mode != TopSided) {
						if (center) {
							new_position.x = collisions_x[i]->left() - dimensions.x / 2;
						}
						else {
							new_position.x = collisions_x[i]->left();
						}
						friction = collisions_x[i]->get_friction().y;
						col_x_found = true;
					}
				}
				else {
					//from right
					if (collisions_x[i]->collision_mode != TopSided) {
						if (center) {
							new_position.x = collisions_x[i]->right() + dimensions.x / 2 - offset.x;
						}
						else {
							new_position.x = collisions_x[i]->right() + dimensions.x - offset.x;
						}
						friction = collisions_x[i]->get_friction().y;
						col_x_found = true;
					}
				}
				entity->on_collision(collisions_x[i]);
				collisions_x[i]->get_entity()->on_collision_x(this);
			}
			if (!col_x_found) {
				new_position.x = entity->transform->get_position().x;
			}
			else {
				if (phys != nullptr) {
					phys->set_velocity_x(-phys->get_velocity().x * bounce);
					phys->apply_counter_force(sf::Vector2f(0, friction));
				}
			}
			entity->transform->set_position(new_position);

		}


		if (!constraint_y) {
			entity->transform->translate(sf::Vector2f(0, movement.y));

			auto collisions_y = check_for_collisions();
			friction = 0;

			for (int i = 0; i < collisions_y.size(); i++) {
				if (movement.y > 0) {
					//hitting collider from the top side

					bool can_collide = false;
					if (collisions_y[i]->collision_mode != TopSided) {
						can_collide = true;
					}
					else if (collisions_y[i]->collision_mode == TopSided) {
						if (bottom() <= collisions_y[i]->top() + 2) {
							can_collide = true;
						}
					}

					if (can_collide) {
						if (center) {
							new_position.y = collisions_y[i]->top() - dimensions.y / 2 - offset.y;
						}
						else {
							new_position.y = collisions_y[i]->top() - dimensions.y - offset.y;
						}

						friction = collisions_y[i]->get_friction().x;
						col_y_found = true;
						phys->restart_gravity();
					}
				}
				else {
					//from bottom
					if (collisions_y[i]->collision_mode != TopSided) {
						if (center) {
							new_position.y = collisions_y[i]->bottom() + dimensions.y / 2;
						}
						else {
							new_position.y = collisions_y[i]->bottom();
						}
						friction = collisions_y[i]->get_friction().x;
						col_y_found = true;
					}
				}

				entity->on_collision(collisions_y[i]);
				collisions_y[i]->get_entity()->on_collision_y(this);
			}
			if (!col_y_found) {
				new_position.y = entity->transform->get_position().y;
			}

			else {
				if (phys != nullptr) {
					phys->set_velocity_y(-phys->get_velocity().y * bounce);
					phys->apply_counter_force(sf::Vector2f(friction,0));
				}
			}
		}
	}

	entity->transform->set_position(new_position);

}

float BoxCollider::left() {
	if (center) {
		return entity->transform->get_position().x - dimensions.x / 2 + offset.x;
	}
	//not center
	return entity->transform->get_position().x + offset.x;
}
float BoxCollider::right() {
	if (center) {
		return entity->transform->get_position().x + dimensions.x / 2 + offset.x;
	}
	//not center
	return entity->transform->get_position().x + dimensions.x + offset.x;
}
float BoxCollider::top() {
	if (center) {
		return entity->transform->get_position().y - dimensions.y / 2 + offset.y;
	}
	//not center
	return entity->transform->get_position().y + offset.y;
}
float BoxCollider::bottom() {
	if (center) {
		return entity->transform->get_position().y + dimensions.y / 2 + offset.y;
	}
	//not center
	return entity->transform->get_position().y + dimensions.y + offset.y;
}