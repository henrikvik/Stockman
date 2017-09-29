#include "../Misc/Card.h"

using namespace Logic;

Card::Card()
{
	m_name = "";
	m_description = "";
}

Card::Card(std::string name, std::string texture, std::string description, std::vector<int> upgradesID, DirectX::SimpleMath::Vector2 texStart, DirectX::SimpleMath::Vector2 texEnd, bool isEffect)
{
	m_name = name;
	m_texture = texture;
	m_description = description;
	m_upgradesID = upgradesID;
	m_TexStart = texStart;
	m_TexEnd = texEnd;
	m_isEffect = isEffect;
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
const std::vector<int>& Card::getUpgradesID() const
{ 
	return m_upgradesID;
}

DirectX::SimpleMath::Vector2 Logic::Card::getTexStart() const
{
	return m_TexStart;
}

DirectX::SimpleMath::Vector2 Logic::Card::getTexEnd() const
{
	return m_TexEnd;
}
