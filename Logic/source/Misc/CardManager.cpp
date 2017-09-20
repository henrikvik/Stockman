#include "../Misc/CardManager.h"
#include <Misc\FileLoader.h>

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
		cards.push_back(Card(struc.strings.at("cardName"), struc.strings.at("texture"), struc.strings.at("description"), upgrades, texStart, texEnd)); //something wrong here
	}
}

void CardManager::restart()
{
	clear();
	init();
}

void Logic::CardManager::createDeck(int nrOfEach)
{
}

void Logic::CardManager::pickThree(int hp)
{
}

void Logic::CardManager::shuffle()
{
}

void Logic::CardManager::pick(int i)
{
}

//Card* CardManager::getRandomCard() 
//{
//	Card choosenCards[3];
//	int numbersToSkip[2] = { -1, -1 };
//	int choosen = 0;
//	while (choosen < 3)
//	{
//		int picked = rand() % cards.size() + 1;
//		if ((numbersToSkip[0] != picked && numbersToSkip[1] != picked))
//		{
//			choosenCards[choosen] = cards.at(picked);
//
//			numbersToSkip[choosen] = picked;
//			choosen++;
//		}
//	}
//	int number = rand() % cards.size() + 1;
//	return &cards[number];
//}