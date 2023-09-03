#include "ColourGrading.h"
#include "../Game.h"

ColourGrading::ColourGrading(std::string name) : Shader(name) {
	setUniform("u_strength", strength);
}

void ColourGrading::update() {

	setUniform("u_texture", game->get_display().getTexture());
	setUniform("u_time", (float)game->time);
}
