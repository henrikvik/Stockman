#include "../Misc/CardManager.h"

using namespace Logic;

CardManager::CardManager()
{
	
}

CardManager::~CardManager() { }

void CardManager::clear()
{
	cards.clear();
}

void CardManager::init() 
{
	for (int i = 0; i < 10; i++)
		cards.push_back(Card());
}

void CardManager::restart()
{
	clear();
	init();
}

Card* CardManager::getRandomCard() 
{
	int number = rand() % cards.size() + 1;
	return &cards[number];
}