#include "DialogueCard.h"

DialogueCard::DialogueCard() {
	this->content = "no string provided";
}

DialogueCard::DialogueCard(std::string content) {
	this->content = content;
}