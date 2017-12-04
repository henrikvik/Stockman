#ifndef CARD_H
#define CARD_H

#include <d3d11.h>
#include <SimpleMath.h>

#include <string>
#include <vector>

#include "../../include/Misc/GUI/Sprite.h"

// Why is this a class?
// There's no functions only get-functions, would be more fitting as a struct

namespace Logic
{
	class Card
	{
	public:
        enum StatusType {
            EFFECT, UPGRADE
        };

        enum CardCategory
        {
            ATTACK, DEFENCE, UTILITY
        };

		Card(std::string name, std::string texture, std::string description,
            std::vector<int> m_statusIDs, DirectX::SimpleMath::Vector2 texStart,
            DirectX::SimpleMath::Vector2 texEnd, int statusType, int category, int numberOf);
		~Card();

		std::string getName() const;
		std::string getTexture() const;
		std::string getDescription() const;

        void setIconPos(float x, float y, float width, float height);
        void setbackgroundPos(float x, float y, float width, float height);
        void setCardTextPos(float posX);

		const std::vector<int>& getStatusIds() const;
		StatusType getStatusType() const;
        CardCategory getCategory() const;
        void setAlpha(float alpha);
        int getNumberOf();

		DirectX::SimpleMath::Vector2 getTexStart() const;
		DirectX::SimpleMath::Vector2 getTexEnd() const;

        virtual void render() const;
	private:
        void parseText();

		std::string m_name;
		std::string m_texture;
		std::string m_description;

		std::vector<int> m_statusIds;
        int m_numberOf;

		DirectX::SimpleMath::Vector2 m_texStart;
		DirectX::SimpleMath::Vector2 m_texEnd;

        CardCategory m_category;
        StatusType m_statusType;

        Sprite m_cardBackground;

        Sprite m_icon;

        std::vector<TextRenderInfo> m_text;
        //std::vector<DirectX::SimpleMath::Vector2> textPos

	};
}

#endif // !CARD_H