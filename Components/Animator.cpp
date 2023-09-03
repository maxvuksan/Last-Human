#include "Animator.h"
#include "AnimationClip.h"
#include "../Entity.h"
#include "../Game.h"

Animator::Animator(Entity* entity): Component(entity)
{
	current_frame = 0;
}

AnimationClip& Animator::get_state(std::string state) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i] == state) {
			return clips[i];
		}
	}
}

void Animator::define_state(std::string name, AnimationClip clip) {
	clips.push_back(clip);
	states.push_back(name);

	if (clips.size() == 1) {
		change_state(name);
		previous_state = name;
	}
}

void Animator::change_state(std::string state) {

	if (state == current_state) {
		return;
	}

	previous_state = current_state;

	current_frame = 0;
	frame_delay_tracked = 0;
	current_state = state;

	for (int i = 0; i < states.size(); i++) {
		if (states[i] == state) {
			current_clip = clips[i];

			current_texture.loadFromFile(current_clip.file_name);
			sprite.setTexture(current_texture);
			sprite.setOrigin(sf::Vector2f(current_clip.frame_dimensions.x / 2, current_clip.frame_dimensions.y / 2));
			sprite.setTextureRect(sf::IntRect(current_clip.frames[current_frame].index * current_clip.frame_dimensions.x, 0, current_clip.frame_dimensions.x, current_clip.frame_dimensions.y));
			return;
		}
	}
	throw "Animation " + state + " could not be found";


}

void Animator::draw(sf::RenderTexture& surface) {

	if(frame_delay_tracked > current_clip.frames[current_frame].duration/(float)50) {
		current_frame++;
		frame_delay_tracked = 0;

		if (current_frame >= current_clip.frames.size()) {
			if (!current_clip.loop) {
				change_state(previous_state);
			}
			else {
				current_frame = 0;
			}
		}
		sprite.setTextureRect(sf::IntRect(current_clip.frames[current_frame].index * current_clip.frame_dimensions.x, 0, current_clip.frame_dimensions.x, current_clip.frame_dimensions.y));
	}

	sprite.setPosition(entity->transform->get_draw_position());
	surface.draw(sprite);

	frame_delay_tracked += entity->game->delta_time;
}
