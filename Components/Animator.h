#pragma once
#include "AnimationClip.h"

class Animator : public Component {

	public:
		std::vector<AnimationClip> clips;
		std::vector<std::string> states;

		sf::Sprite sprite;
		sf::Texture current_texture;
		AnimationClip current_clip;
		std::string current_state;
		std::string previous_state;

		int render_layer = 0;
		int current_frame = 0;
		double frame_delay_tracked = 0;

	public:
		Animator(Entity*);
		void draw(sf::RenderTexture& surface) override;
		void define_state(std::string name, AnimationClip clip); //defines a state with a corrosponding animation clip
		void change_state(std::string state); //switches to given state name
		AnimationClip& get_state(std::string state);
};
