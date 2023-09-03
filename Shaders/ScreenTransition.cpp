#include "ScreenTransition.h"
#include "../Game.h"

ScreenTransition::ScreenTransition(std::string name) : Shader(name) 
{
	setUniform("u_colour", sf::Glsl::Vec4(colour));
}

void ScreenTransition::update() 
{
	setUniform("u_texture", game->get_display().getTexture());
}