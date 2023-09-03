#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Entity;


class Component { 

	protected:
		int render_layer = 0;
		Entity* entity;
		bool active = true; //on by default
	public:

		void set_active(bool state) { active = state; }
		bool get_active() { return active; }

		virtual ~Component() {}
		Component(Entity* entity);
		Entity* get_entity() { return entity; }
		virtual void awake(){}
		virtual void update(){} //no rendering
		virtual void draw(sf::RenderTexture&){} //a surface to render to

		int get_render_layer();
		void set_render_layer(int);
};