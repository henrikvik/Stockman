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
		Card();
		Card(std::string name, std::string texture, std::string description, std::vector<int> upgradesID, DirectX::SimpleMath::Vector2 texStart, DirectX::SimpleMath::Vector2 texEnd, bool isEffect);
		~Card();

		std::string getName() const;
		std::string getTexture() const;
		std::string getDescription() const;
		const std::vector<int>& getUpgradesID() const;
		DirectX::SimpleMath::Vector2 getTexStart() const;
		DirectX::SimpleMath::Vector2 getTexEnd() const;

	private:
		std::string m_name;
		std::string m_texture;
		std::string m_description;
		std::vector<int> m_upgradesID;
		DirectX::SimpleMath::Vector2 m_TexStart;
		DirectX::SimpleMath::Vector2 m_TexEnd;
		bool m_isEffect;
	};
}

#endif // !CARD_H
