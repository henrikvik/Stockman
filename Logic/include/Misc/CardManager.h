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
		void pickThree(bool hp);
		void shuffle(int times);
		Card pick(int i);

		/*Card* getRandomCard();*/
	private:
		Card healthPack;
		std::vector<Card> m_cards;
		std::vector<int> m_deck;
		int m_hand[3];
	};
}


#endif // !CARDMANAGER_H
