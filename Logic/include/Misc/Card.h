#ifndef CARD_H
#define CARD_H

#include <string>

namespace Logic
{
	class Card
	{
	public:
		Card();
		Card(std::string name, std::string description, int statusID);
		Card(const Card& other) = delete;
		Card* operator=(const Card& other) = delete;
		~Card();

		std::string getName() const;
		std::string getDescription() const;
		int getStatusID() const;

	private:
		std::string m_name;
		std::string m_description;
		int m_statusID;
	};
}

#endif // !CARD_H
