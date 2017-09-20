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

		void createDeck(int nrOfEach);
		void pickThree(bool damaged);
		void shuffle(int times);
		Card pick(int cardIndex);

		/*Card* getRandomCard();*/
	private:
		const int healthPack = 0;
		static const int handSize = 3;
		std::vector<Card> m_cards;
		std::vector<int> m_deck;
		int m_hand[CardManager::handSize];
	};
}


#endif // !CARDMANAGER_H
