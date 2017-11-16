#include <Misc/Card.h>

using namespace Logic;

Card::Card(std::string name, std::string texture, std::string description,
    std::vector<int> statusIds, DirectX::SimpleMath::Vector2 texStart,
    DirectX::SimpleMath::Vector2 texEnd, int statusType, int category)
    :   m_name(name), m_texture(texture), m_description(description),
        m_statusIds(statusIds), m_texStart(texStart), m_texEnd(texEnd),
        m_statusType(static_cast<StatusType> (statusType)), 
        m_category(static_cast<CardCategory>(category)){

}

Card::~Card() { }

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

Card::CardCategory Card::getCategory() const
{
    return m_category;
}
