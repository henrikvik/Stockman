#include "../Misc/Card.h"

using namespace Logic;

Card::Card()
{
	m_name = "";
	m_texture = "";
	m_description = "";
	m_texStart = DirectX::SimpleMath::Vector2(0.0f,0.0f);
	m_texEnd = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	m_statusType = UPGRADE;
}

Card::Card(std::string name, std::string texture, std::string description,
    std::vector<int> upgradesID, DirectX::SimpleMath::Vector2 texStart,
    DirectX::SimpleMath::Vector2 texEnd, bool isEffect)
    :   m_name(name), m_texture(texture), m_description(description),
        m_statusIds(m_statusIds), m_texStart(texStart), m_texEnd(texEnd){

}

Card::~Card() 
{ 
}

std::string Card::getName() const 
{ 
	return m_name; 
}
std::string Logic::Card::getTexture() const
{
	return m_texture;
}
std::string Card::getDescription() const 
{ 
	return m_description; 
}
const std::vector<int>& Card::getStatusIds() const
{ 
	return m_statusIds;
}

DirectX::SimpleMath::Vector2 Logic::Card::getTexStart() const
{
	return m_texStart;
}

DirectX::SimpleMath::Vector2 Logic::Card::getTexEnd() const
{
	return m_texEnd;
}

Card::StatusType Card::getStatusType() const
{
	return m_statusType;
}
