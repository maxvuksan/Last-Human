#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include <iostream>
#include "../COMPONENTS.h"


class Player : public Entity {
	protected:

		std::map<std::string, short> keys;
		std::map<std::string, float> input_buffers;
		std::map<std::string, float> trigger_buffers;
		std::map<std::string, bool> triggers; //final inputs, after all leeway is considered

		float trigger_leeway = 2;

		float speed = 22;
		float climb_speed = 10;
		float jump_height = 42;
		float variable_jump_height = 2.0f;
		float jump_holding = 0;

		float gravity = 3;
		float gravity_min = 1.4f; //when the player is holding jump
		float gravity_max = 5; //when player is falling
		
		float jump_delay_amount = 0.2f;
		float jump_delay;
		float pre_jump_amount = 1.0;
		float post_jump_amount = 0.8;
		float post_grab_amount = 0.4;
		float pre_jump; //holds player input for jump_leeway amount of time before touching ground
		float post_jump; //detects player input for jump_leeway amount of time, after leaving ground
		float post_grab;

		Animator* anim;
		PhysicsBody* pb;

		BoxCollider* collider;
		BoxCollider* left_ground_check;
		BoxCollider* right_ground_check;
		
		bool grounded;
		bool grabbing_left = false;
		bool grabbing_right = false;
		bool grabbing_falling_block_left = false;
		bool grabbing_falling_block_right = false;

		float falling_block_previous_frame_y;
		Collider* current_grabbed_collider;

		bool triggered_left; //is the player considered next to a wall
		bool triggered_right; 
		
		BoxCollider* bottom_left_wall_check;
		BoxCollider* bottom_right_wall_check;
		BoxCollider* lower_left_wall_check;
		BoxCollider* lower_right_wall_check;
		BoxCollider* upper_left_wall_check;
		BoxCollider* upper_right_wall_check;

		float time_outside_room;
		float outside_room_threshhold = 3;
	public:
		bool interacting; //talking to npc

		Player();
		~Player();

		bool grounded_by(Collider* collider); //checks if a collider is within the players ground check triggers

		void awake();
		void determine_inputs();
		void update() override;
		void manage_events(sf::Event event) override;
		void in_trigger(Collider*) override;

		void on_remove() override;
		void define_checker(BoxCollider*& variable, sf::Vector2f dimensions, sf::Vector2f offset);
		void jump();
		void check_out_of_bounds();
};