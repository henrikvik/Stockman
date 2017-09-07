#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "Card.h"
#include <vector>
#include <random>

namespace Logic
{
	class CardManager
	{
	public:
		CardManager();
		CardManager(const CardManager& other) = delete;
		CardManager* operator=(const CardManager& other) = delete;
		~CardManager();

		void clear();
		void init();
		void restart();

		Card* getRandomCard();
	private:
		std::vector<Card> cards;
	};
}


#endif // !CARDMANAGER_H
