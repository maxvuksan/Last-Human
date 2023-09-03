#pragma once
#include <SFML/Graphics.hpp>

class Game;
class Shader : public sf::Shader {

	protected:
		Game* game;

	public:
		Shader(std::string name);
	
		void define_game_parent(Game* game);
		virtual void update(){}
};