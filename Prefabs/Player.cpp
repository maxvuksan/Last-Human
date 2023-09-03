#include "Player.h"
#include "../COMPONENTS.h"
#include "../Game.h"
#include <iostream>
#include "../Managers/StageManager.h"

Player::Player() 
{
	tag = "player";
	collision_layer = 1;
	time_outside_room = 0;

	input_buffers.insert(std::make_pair("left", 0));
	input_buffers.insert(std::make_pair("right", 0));
	input_buffers.insert(std::make_pair("up", 0));

	trigger_buffers.insert(std::make_pair("grounded_left", 0));
	trigger_buffers.insert(std::make_pair("grounded_right", 0));
	trigger_buffers.insert(std::make_pair("upper_left_wall", 0));
	trigger_buffers.insert(std::make_pair("lower_left_wall", 0));
	trigger_buffers.insert(std::make_pair("upper_right_wall", 0));
	trigger_buffers.insert(std::make_pair("lower_right_wall", 0));

	keys.insert(std::make_pair("left", 0));
	keys.insert(std::make_pair("right", 0));
	keys.insert(std::make_pair("up", 0));
	keys.insert(std::make_pair("down", 0));
	keys.insert(std::make_pair("grab", 0));

	triggers.insert(std::make_pair("grounded_left", false));
	triggers.insert(std::make_pair("grounded_right", false));
	triggers.insert(std::make_pair("upper_left_wall", false));
	triggers.insert(std::make_pair("lower_left_wall", false));
	triggers.insert(std::make_pair("upper_right_wall", false));
	triggers.insert(std::make_pair("lower_right_wall", false));

	triggers.insert(std::make_pair("any_left_wall", false));
	triggers.insert(std::make_pair("any_right_wall", false));
	triggers.insert(std::make_pair("grounded", false));
}

void Player::awake() {

	anim = add_component<Animator>(true);

	anim->define_state("idle", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 0, 2));
	anim->get_state("idle").set_duration(150);
	anim->define_state("fall_up", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 6, 7));
	anim->define_state("fall_down", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 7, 8));
	anim->define_state("run", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 2, 6));
	anim->get_state("run").set_duration({90,60,90,60});
	anim->define_state("grab", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 9, 10));
	anim->define_state("grab_high", AnimationClip("Images/Tiles/player.png", sf::Vector2f(14, 20), 10, 11));

	collider = add_component<BoxCollider>(true);
	collider->set_dimensions(sf::Vector2f(8, 20));
	pb = add_component<PhysicsBody>(true);

	transform->set_position(game->get_stage_manager()->current_room->get_spawnpoint_world_position());

	define_checker(left_ground_check, sf::Vector2f(2, 4), sf::Vector2f(-3, 10));
	define_checker(right_ground_check, sf::Vector2f(2, 4), sf::Vector2f(3, 10));

	define_checker(upper_left_wall_check, sf::Vector2f(3, 3), sf::Vector2f(-5, -2));
	define_checker(upper_right_wall_check, sf::Vector2f(3, 3), sf::Vector2f(5, -2));
	define_checker(lower_left_wall_check, sf::Vector2f(3, 6), sf::Vector2f(-5, 4));
	define_checker(lower_right_wall_check, sf::Vector2f(3, 6), sf::Vector2f(5, 4));
	define_checker(bottom_left_wall_check, sf::Vector2f(3, 2), sf::Vector2f(-5, 7));
	define_checker(bottom_right_wall_check, sf::Vector2f(3, 2), sf::Vector2f(5, 7));
}

void Player::define_checker(BoxCollider*& variable, sf::Vector2f dimensions, sf::Vector2f offset) {
	
	Entity* ptr = new Entity();
	game->add_entity(ptr);

	ptr->transform->set_parent(this->transform);
	ptr->add_component<BoxCollider>();
	ptr->collision_layer = 2;
	ptr->transform->set_local_position(offset);
	variable = ptr->get_component<BoxCollider>();
	variable->set_dimensions(dimensions);
	variable->is_trigger = true;
}

void Player::determine_inputs() {

	if (lower_left_wall_check->triggered) { trigger_buffers["lower_left_wall"] = trigger_leeway; }
	if (lower_right_wall_check->triggered) { trigger_buffers["lower_right_wall"] = trigger_leeway; }
	if (upper_left_wall_check->triggered) { trigger_buffers["upper_left_wall"] = trigger_leeway; }
	if (upper_right_wall_check->triggered) { trigger_buffers["upper_right_wall"] = trigger_leeway; }
	if (left_ground_check->triggered) { trigger_buffers["grounded_left"] = post_jump_amount; }
	if (right_ground_check->triggered) { trigger_buffers["grounded_right"] = post_jump_amount; }

	if (lower_left_wall_check->triggered || upper_left_wall_check->triggered || bottom_left_wall_check->triggered) {
		triggers["any_left_wall"] = true;
	}
	else {
		triggers["any_left_wall"] = false;
		grabbing_left = false;
		grabbing_falling_block_left = false;
	}
	if (lower_right_wall_check->triggered || upper_right_wall_check->triggered || bottom_right_wall_check->triggered) {
		triggers["any_right_wall"] = true;
	}
	else {
		triggers["any_right_wall"] = false;
		grabbing_right = false;
		grabbing_falling_block_right = false;
	}

	if (keys["grab"]) {
		if (triggers["any_left_wall"]) {
			grabbing_left = true;
			post_grab = post_grab_amount;
		}
		else if (triggers["any_right_wall"]) {
			grabbing_right = true;
			post_grab = post_grab_amount;
		}
	}

	//Is the player grounded?
	if (trigger_buffers["grounded_left"] > 0 || trigger_buffers["grounded_right"] > 0) {
		if (!triggers["grounded"]) {
			if (pb->get_velocity().y > 16) {
				game->play_sound("land.wav", 2.5f);

				auto* impact = new VFX::SurfaceImpact();
				game->add_entity(impact);
				impact->transform->set_position(sf::Vector2f(transform->get_position().x, collider->bottom()));
			}
		}
		triggers["grounded"] = true;

		if (pre_jump > 0) {
			jump();
			pre_jump = 0;
		}
	}
	else {
		triggers["grounded"] = false;
	}

	std::map<std::string, float>::iterator it;
	for (it = trigger_buffers.begin(); it != trigger_buffers.end(); it++)
	{
		//it->first //key string
		//it->second //value

		if (it->second > 0) {
			it->second -= game->delta_time;
			triggers[it->first] = true;
		}
		else {
			triggers[it->first] = false;
		}
	}
}

void Player::update() {

	determine_inputs();
		
	//flipping animator sprite
	if (pb->get_velocity().x > 0) {
		anim->sprite.setScale(sf::Vector2f(1, 1));
	}
	else if (pb->get_velocity().x < 0) {
		anim->sprite.setScale(sf::Vector2f(-1, 1));
	}



	if (grabbing_left || grabbing_right) {

		pb->set_velocity_x(0);
		bool on_oil = false;
		if (grabbing_left) {
			anim->sprite.setScale(sf::Vector2f(-1, 1));

			//against oil
			if (triggers["lower_left_wall"])
			{
				for (int i = 0; i < lower_left_wall_check->get_colliders_in_trigger().size(); i++) 
				{
					if (lower_left_wall_check->get_colliders_in_trigger()[i]->collision_layer == 3) {
						on_oil = true;
						pb->set_gravity(gravity);
						break;
					}
				}
			}
			if (!on_oil && triggers["upper_left_wall"]) {
				for (int i = 0; i < upper_left_wall_check->get_colliders_in_trigger().size(); i++) {
					if (upper_left_wall_check->get_colliders_in_trigger()[i]->collision_layer == 3) {
						on_oil = true;
						pb->set_gravity(gravity);
						break;
					}
				}
			}
			if(!on_oil) {
				pb->set_gravity(0);
				pb->set_velocity_y((keys["up"] + keys["down"]) * climb_speed);
			}

			if (bottom_left_wall_check->triggered && !lower_left_wall_check->triggered && !upper_left_wall_check->triggered) {
				pb->set_velocity_y(-5);
				pb->set_velocity_x(-2);
				transform->set_position_y(bottom_left_wall_check->get_colliders_in_trigger()[0]->top() - collider->get_dimensions().y / 2);
				transform->set_position_x(bottom_left_wall_check->get_colliders_in_trigger()[0]->right() + collider->get_dimensions().x / 2 - 1);
			}
			else {
				//attaching player to moving block
				Collider* col = nullptr;
				if (lower_left_wall_check->triggered) {
					col = lower_left_wall_check->get_colliders_in_trigger()[0];
				}
				else if(upper_left_wall_check->triggered) {
					col = upper_left_wall_check->get_colliders_in_trigger()[0];
				}
				if (col != current_grabbed_collider) {
					grabbing_falling_block_left= false;
				}
				current_grabbed_collider = col;

				if (col != nullptr) {
					transform->set_position_x(round(col->right() + collider->get_dimensions().x / 2));

					if (col->get_entity()->tag == "falling_block") {

						FallingBlock* block = dynamic_cast<FallingBlock*>(col->get_entity());
						block->trigger();

						if (grabbing_falling_block_left) {
							collider->move(sf::Vector2f(0, block->transform->get_position().y - falling_block_previous_frame_y));
						}
						falling_block_previous_frame_y = block->transform->get_position().y;
						grabbing_falling_block_left = true;
					}


					if (!lower_left_wall_check->triggered && !bottom_left_wall_check->triggered) {
						anim->change_state("grab_high");
					}
					else {
						anim->change_state("grab");
					}
				}
			}
		}
		else {
			anim->sprite.setScale(sf::Vector2f(1, 1));

			//against oil
			if (triggers["lower_right_wall"])
			{
				for (int i = 0; i < lower_right_wall_check->get_colliders_in_trigger().size(); i++) {

					if (lower_right_wall_check->get_colliders_in_trigger()[i]->collision_layer == 3) {
						on_oil = true;
						pb->set_gravity(gravity);
						break;
					}
				}
			}
			if (!on_oil && triggers["upper_right_wall"]) {
				for (int i = 0; i < upper_right_wall_check->get_colliders_in_trigger().size(); i++) {

					if (upper_right_wall_check->get_colliders_in_trigger()[i]->collision_layer == 3) {
						on_oil = true;
						pb->set_gravity(gravity);
						break;
					}
				}
			}
			if (!on_oil) {
				pb->set_gravity(0);
				pb->set_velocity_y((keys["up"] + keys["down"]) * climb_speed);
			}

			if (bottom_right_wall_check && !lower_right_wall_check->triggered && !upper_right_wall_check->triggered) {
				pb->set_velocity_y(-5);
				pb->set_velocity_x(2);
				transform->set_position_y(bottom_right_wall_check->get_colliders_in_trigger()[0]->top() - collider->get_dimensions().y / 2);
				transform->set_position_x(bottom_right_wall_check->get_colliders_in_trigger()[0]->left() - collider->get_dimensions().x/2 + 1);
			}
			else {
				//attaching player to moving block
				Collider* col = nullptr;
				if (lower_right_wall_check->triggered) {
					col = lower_right_wall_check->get_colliders_in_trigger()[0];
				}
				else if(upper_right_wall_check->triggered) {
					col = upper_right_wall_check->get_colliders_in_trigger()[0];
				}
				if (col != current_grabbed_collider) {
					grabbing_falling_block_right = false;
				}
				current_grabbed_collider = col;

				if (col != nullptr) {
					transform->set_position_x(round(col->left() - collider->get_dimensions().x / 2));

					if (col->get_entity()->tag == "falling_block") {

						FallingBlock* block = dynamic_cast<FallingBlock*>(col->get_entity());
						block->trigger();

						if (grabbing_falling_block_right) {
							collider->move(sf::Vector2f(0, block->transform->get_position().y - falling_block_previous_frame_y));
						}
						falling_block_previous_frame_y = block->transform->get_position().y;
						grabbing_falling_block_right = true;
					}

					if (!lower_right_wall_check->triggered && !bottom_right_wall_check->triggered) {
						anim->change_state("grab_high");
					}
					else {
						anim->change_state("grab");
					}
				}
			}
		}
	}
	else {

		pb->set_gravity(gravity);
		pb->add_force(sf::Vector2f(keys["left"] * speed + keys["right"] * speed, 0));


		if (triggers["grounded"]) {

			if (abs(pb->get_velocity().x) > 2) {
				anim->change_state("run");
			}
			else {

				anim->change_state("idle");
			}

			if (!keys["left"] && !keys["right"]) {
				pb->apply_counter_force(sf::Vector2f(5, 0));
			}

		}
		else {
			if (pb->get_velocity().y > 0) {
				anim->change_state("fall_down");
			}
			else {
				anim->change_state("fall_up");
			}
		}
	}

	if (pre_jump > 0) {
		pre_jump -= game->delta_time;
	}
	if (post_jump > 0) {
		post_jump -= game->delta_time;
		jump();
	}
	if (post_grab > 0) {
		post_grab -= game->delta_time;
	}
	if (jump_delay > 0) {
		jump_delay -= game->delta_time;
	}
	if (jump_holding > 0) {
		if (pb->get_gravity() != 0) {
			//float t = jump_holding / variable_jump_height;
			//float lerp_val = gravity_min * t + gravity * (1 - t);
			//pb->add_force(sf::Vector2f(0,-25));
		}
		
		jump_holding -= game->delta_time;
	}
	else {
		if (pb->get_gravity() != 0) {
			pb->set_gravity(gravity);
		}
	}

	if (pb->get_gravity() != 0) {
		if (pb->get_velocity().y > 0) {
			pb->set_gravity(gravity);
			jump_holding = 0;
		}
	}

	check_out_of_bounds();
}

void Player::check_out_of_bounds() {
	time_outside_room += game->delta_time;

	for (int i = 0; i < game->get_stage_manager()->rooms.size(); i++) {
		Room* room = game->get_stage_manager()->rooms[i];

		if (room->get_bounds_trigger()->overlapping(*collider)) {
			time_outside_room = 0;
		}
	}

	if (time_outside_room > outside_room_threshhold) {
		game->remove_entity(this);
	}
}

void Player::in_trigger(Collider* col) {
	if (col->collision_layer == 2) {
		game->remove_entity(this);
	}
}

bool Player::grounded_by(Collider* col) {
	if (left_ground_check->overlapping(*col) || right_ground_check->overlapping(*col)) {
		return true;
	}
	return false;
}


void Player::on_remove() {
	game->play_sound("hit.wav");	
	game->get_scene_manager()->player_death_delay(game->get_stage_manager()->current_room);
}

void Player::jump() {

	if (jump_delay <= 0) {
		pb->set_gravity(gravity);
		keys["grab"] = false;

		if ((triggers["grounded"]) && !(grabbing_left || grabbing_right))
		{
			game->play_sound("jump.wav", 5);
			pb->restart_gravity();

			jump_holding = variable_jump_height;
			pb->set_velocity_y(-jump_height);
		}
		else
		{
			if (triggers["any_left_wall"] || triggers["any_right_wall"])
			{
				if (((grabbing_left || grabbing_right)) && keys["up"] && ((!keys["left"] && grabbing_right) || (!keys["right"] && grabbing_left))) {
					if (!(keys["left"] && keys["right"])) {
						pb->restart_gravity();
						pb->set_velocity_y(-jump_height);
					}
				}
				else {
					int direction = 0;
					if (triggers["any_left_wall"]) {
						direction = 1;
					}
					else if (triggers["any_right_wall"]) {
						direction = -1;
					}

					if (direction != 0) {
						pb->restart_gravity();
						pb->set_velocity_x(direction * jump_height);
						pb->set_velocity_y(-jump_height);

						if (!grabbing_left && !grabbing_right) {
							int ori = 90;
							float position_x = collider->right();
							if (direction == 1) {
								position_x = collider->left();
							}
							auto* impact = new VFX::SurfaceImpact(ori);
							game->add_entity(impact);
							impact->transform->set_position(sf::Vector2f(position_x, collider->bottom()));
						}
					}
				}
			}
		}
		grabbing_left = false;
		grabbing_right = false;
		post_grab = 0; 
		post_jump = 0;
		pre_jump = 0;
		jump_delay = jump_delay_amount;
	}	
}

void Player::manage_events(sf::Event event) {

	if (interacting) {
		jump_holding = 0;
		keys["up"] = 0;
		keys["left"] = 0;
		keys["down"] = 0;
		keys["right"] = 0;
		keys["grab"] = 0;

		if (grabbing_left || grabbing_right) {
			pb->restart_gravity();
		}
		grabbing_left = false;
		grabbing_right = false;
		grabbing_falling_block_left = false;
		grabbing_falling_block_right = false;
		return;
	}

	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::C:
				pre_jump = pre_jump_amount;
				jump();
				break;
			case sf::Keyboard::Up:
				keys["up"] = -1;
				break;
			case sf::Keyboard::Left:
				keys["left"] = -1;
				break;
			case sf::Keyboard::Down:
				keys["down"] = 1;
				break;
			case sf::Keyboard::Right:
				keys["right"] = 1;
				break;
			case sf::Keyboard::Z:
				keys["grab"] = 1;
				break;
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::C:
				jump_holding = 0;
				break;
			case sf::Keyboard::Up:
				keys["up"] = 0;
				break;
			case sf::Keyboard::Left:
				keys["left"] = 0;
				break;
			case sf::Keyboard::Down:
				keys["down"] = 0;
				break;
			case sf::Keyboard::Right:
				keys["right"] = 0;
				break;
			case sf::Keyboard::Z:
				keys["grab"] = 0;

				if (grabbing_left || grabbing_right) {
					pb->restart_gravity();
				}
				grabbing_left = false;
				grabbing_right = false;
				grabbing_falling_block_left = false;
				grabbing_falling_block_right = false;
				break;
		}
	}
}

Player::~Player() {}