#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "Card.h"
#include <vector>
#include <random>

#include <Misc\FileLoader.h>

namespace Logic
{
    class Player;

	class CardManager
	{
	public:
        enum CardCondition
        {
            NEVER_REMOVE, IN_DECK, TAKEN
        };

		CardManager();
		CardManager(const CardManager& other) = delete;
		CardManager* operator=(const CardManager& other) = delete;
		~CardManager();

		void clear();
		void init();
		void restart();

        // returns true if a card is applied (index >= 0)
        bool pickAndApplyCard(Player &player, int cardIndex);
        void handleCard(Player &player, Card const &card);

		void createDeck(int nrOfEach);
		void pickThree(bool damaged);
		void shuffle();
		Card pick(int cardIndex);
	private:
		static const int HEALTH_PACK;
		static const int HAND_SIZE;

        void createCard(CardCondition cond, FileLoader::LoadedStruct const &struc);
        
        // int = card index
        std::vector<int>                            m_hand;
		std::vector<std::pair<CardCondition, int>>  m_deck;
        std::vector<Card>                           m_cards;
	};
}


#endif // !CARDMANAGER_H
