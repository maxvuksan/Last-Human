#include "AnimationClip.h"


AnimationClip::AnimationClip() {}

AnimationClip::AnimationClip(std::string file_name, sf::Vector2f frame_dimensions, int min, int max, bool loop) {

	this->loop = loop;
	this->frame_dimensions = frame_dimensions;
	this->file_name = file_name;
	for (int i = min; i < max; i++) {

		Frame frame = { i, 30 };
		frames.push_back(frame);
	}
}

void AnimationClip::set_duration(int dur) {
	for (int i = 0; i < frames.size(); i++) {
		frames[i].duration = dur;
	}
}

void AnimationClip::set_duration(std::vector<int> dur) {
	for (int i = 0; i < frames.size(); i++) {
		frames[i].duration = dur[i];
	}
}

