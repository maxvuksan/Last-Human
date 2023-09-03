#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Game;
#include "COMPONENTS.h"


class Entity {

	protected:
		int render_layer = 0;
		bool remove_started = false;
		float remove_time_tracked = 0;

	public:
		bool removed = false;
		std::string tag = "";
		int collision_layer = 0;

		std::vector<Component*> components;
		Transform* transform;
		Game* game;


		Entity();
		Entity(sf::Vector2f position);
		virtual ~Entity();

		void define_game_parent(Game* game);

		void remove_in(float remove_time_tracked);
		void track_remove_time();

		virtual void awake(){} //is called after the entity is defined 
		virtual void update(){}
		virtual void draw(sf::RenderTexture& surface){}
		virtual void manage_events(sf::Event event){}

		virtual void on_remove() {} //is called when the object is removed
		virtual void on_collision(Collider* other){}
		virtual void on_collision_x(Collider* other){} //purley checking one axis
		virtual void on_collision_y(Collider* other) {}

		virtual void on_trigger(Collider* other){} //an collider entered THIS entity's trigger
		virtual void in_trigger(Collider* other){} //you have entered an entities trigger

		int get_render_layer();
		void set_render_layer(int);

		template <typename T> //no return value
		void add_component() {

			T* new_component = new T(this);
			if (dynamic_cast<Component*>(new_component) != NULL) { //is the given class a valid component?
				components.push_back(new_component);
				return;
			}
			//else, not a valid component
			throw std::invalid_argument("<T> is not a valid component class");
		}

		template <typename T> //we want a pointer to the return value
		T* add_component(bool return_value) {

			T* new_component = new T(this);
			if (dynamic_cast<Component*>(new_component) != NULL) { //is the given class a valid component?
				components.push_back(new_component);

				return new_component;
			}
			//else, not a valid component
			throw std::invalid_argument("<typname T> is not a valid component class");
		}

		template <typename T>
		T* get_component() { //returns a pointer to said component

			for (int i = 0; i < components.size(); i++)
			{
				//testing if the component at index: i can be casted to a pointer of the desired type
				T* comp = dynamic_cast<T*>(components[i]); //if the cast can not be done, it returns NULL
				if (comp != NULL) {
					return comp; //if so we can return the pointer to that component
				}
			}
			return nullptr;
		}



};