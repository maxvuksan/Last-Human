#include "Shader.h"
#include "../Game.h"

Shader::Shader(std::string name) {
	if (!sf::Shader::isAvailable())
	{
		throw "Shader are not available";
	}
	if (!loadFromFile("Shaders/" + name + ".frag", sf::Shader::Fragment))
	{
		throw "error loading fragment shader";
	}
}

void Shader::define_game_parent(Game* game) {
	this->game = game;
}
