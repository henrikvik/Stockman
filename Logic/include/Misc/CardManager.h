#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "Card.h"
#include <vector>
#include <random>

#include <Misc\NonCopyable.h>
#include <Misc\FileLoader.h>

namespace Logic
{
    class Player;

	class CardManager : public NonCopyable
	{
	public:
        enum CardCondition
        {
            NEVER_REMOVE, IN_DECK, TAKEN
        };

		CardManager(int nrOfEach = 1);
		~CardManager();

		void resetDeck();

        // returns true if a card is applied (index >= 0)
        bool pickAndApplyCard(Player &player, int cardIndex);
        void handleCard(Player &player, Card const &card);

		void createDeck(int nrOfEach);
		void pickThreeCards(bool damaged);
		Card pick(int cardIndex);
 
        std::vector<Card*> getHand();

        virtual void render() const;
	private:
		static const int HEALTH_PACK;
        static const int HAND_SIZE;
        static const int NEVER_REMOVE_CARDS;

        void loadDeckFromFile();
        void createCard(CardCondition cond, FileLoader::LoadedStruct const &struc);
		void shuffle();
        
        // int = card index
        std::vector<int>                            m_hand;
		std::vector<std::pair<CardCondition, int>>  m_deck;
        std::vector<Card>                           m_cards;

        //temp
        std::vector<Card*> currenthand;
        void pepperCardsForDraw();
	};
}


#endif // !CARDMANAGER_H
