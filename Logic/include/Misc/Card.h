#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>

namespace Logic
{
	class Card
	{
	public:
        enum StatusType {
            EFFECT, UPGRADE
        };

		Card();
		Card(std::string name, std::string texture, std::string description,
            std::vector<int> m_statusIDs, DirectX::SimpleMath::Vector2 texStart,
            DirectX::SimpleMath::Vector2 texEnd, int statusType);
		~Card();

		std::string getName() const;
		std::string getTexture() const;
		std::string getDescription() const;

		const std::vector<int>& getStatusIds() const;
		StatusType getStatusType() const;

		DirectX::SimpleMath::Vector2 getTexStart() const;
		DirectX::SimpleMath::Vector2 getTexEnd() const;
	private:
		std::string m_name;
		std::string m_texture;
		std::string m_description;

		std::vector<int> m_statusIds;

		DirectX::SimpleMath::Vector2 m_texStart;
		DirectX::SimpleMath::Vector2 m_texEnd;

        StatusType m_statusType;
	};
}

#endif // !CARD_H