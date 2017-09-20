#include "../Misc/CardManager.h"
#include <Misc\FileLoader.h>

using namespace Logic;

CardManager::CardManager()
{
	for (int i = 0; i < sizeof(m_hand); i++)
	{
		m_hand[i] = -1; //is default which is healthpacks
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
		m_cards.push_back(Card(struc.strings.at("cardName"), struc.strings.at("texture"), struc.strings.at("description"), upgrades, texStart, texEnd)); //something wrong here
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
		for (int j = 0; j < m_cards.size(); j++)
		{
			m_deck.push_back(j);
		}
	}
}

void Logic::CardManager::pickThree(bool hp)
{
	int handSize = 3;
	if (!hp)
	{
		handSize = 2;
	}
	for(int i = 0; i < handSize; i++)
	{
		m_hand[i] = m_deck.at(i);
	}
}

void Logic::CardManager::shuffle(int times)
{
	for (int i; i < times; i++)
	{
		std::random_shuffle(m_deck.begin(), m_deck.end(), m_deck);
	}
}

Card Logic::CardManager::pick(int i)
{
	if (m_hand[i] == -1)
	{
		shuffle(1);
		return healthPack;
	}
	else
	{
		m_deck.erase(m_deck.begin() + i);
		shuffle(1);
		Card choosen = m_cards.at(i);

		for (int i = 0; i < sizeof(m_hand); i++)
		{
			m_hand[i] = -1; //is default which is healthpacks
		}

		return choosen;
	}
}