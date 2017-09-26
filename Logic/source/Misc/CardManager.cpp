#include "../Misc/CardManager.h"
#include <Misc\FileLoader.h>

using namespace Logic;

CardManager::CardManager()
{
	for (int i = 0; i < handSize; i++)
	{
		m_hand[i] = -1; //is default
	}
}

CardManager::~CardManager() { }

void CardManager::clear()
{
	m_cards.clear();
}

void CardManager::init() 
{
	std::vector<FileLoader::LoadedStruct> cardFile;
	FileLoader::singleton().loadStructsFromFile(cardFile, "Cards");

	for (auto const& struc : cardFile)
	{
		std::vector<int> upgrades;
		for (int i = 0; i < struc.ints.at("upgradeAmmount"); i++)
		{
			upgrades.push_back(struc.ints.at("upgrade" + std::to_string(i + 1)));
		}

		DirectX::SimpleMath::Vector2 texStart(struc.floats.at("xTexStart"), struc.floats.at("yTexStart"));
		DirectX::SimpleMath::Vector2 texEnd(struc.floats.at("xTexEnd"), struc.floats.at("yTexEnd"));;
		m_cards.push_back(Card(struc.strings.at("cardName"), struc.strings.at("texture"), struc.strings.at("description"), upgrades, texStart, texEnd, struc.ints.at("isEffect"))); //something wrong here
	}
}

void CardManager::restart()
{
	clear();
	init();
}

void Logic::CardManager::createDeck(int nrOfEach)
{
	for (int i = 0; i < nrOfEach; i++)
	{
		for (int j = 1; j < m_cards.size(); j++)
		{
			m_deck.push_back(j);
		}
	}
}

void Logic::CardManager::pickThree(bool damaged)
{
	int ammount = handSize;
	int end = handSize - 1;
	if (damaged)
	{
		m_hand[end] = healthPack;
		ammount--;
	}

	for(int i = 0; i < ammount; i++)
	{
		m_hand[i] = m_deck.at(i);
	}
}

void Logic::CardManager::shuffle(int times)
{
	for (int i = 0; i < times; i++)
	{
		std::random_shuffle(m_deck.begin(), m_deck.end());
	}
}

Card Logic::CardManager::pick(int cardIndex)
{
	if (m_hand[cardIndex] != healthPack)
	{
		m_deck.erase(m_deck.begin() + cardIndex);
	}

	shuffle(1);

	Card choosen = m_cards.at(m_hand[cardIndex]);

	for (int i = 0; i < handSize; i++)
	{
		m_hand[i] = -1; //is default
	}

	return choosen;
}