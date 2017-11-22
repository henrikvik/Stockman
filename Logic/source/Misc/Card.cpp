#include <Misc/Card.h>
#include <comdef.h>

using namespace Logic;

Card::Card(std::string name, std::string texture, std::string description,
    std::vector<int> statusIds, DirectX::SimpleMath::Vector2 texStart,
    DirectX::SimpleMath::Vector2 texEnd, int statusType, int category)
    :   m_name(name), m_texture(texture), m_description(description),
        m_statusIds(statusIds), m_texStart(texStart), m_texEnd(texEnd),
        m_statusType(static_cast<StatusType> (statusType)), 
        m_category(static_cast<CardCategory>(category)),
        m_icon(0.0f, 0.0f, 100.0f, 150.0f, Resources::Textures::iconsheet, FloatRect(texStart, texEnd), 1.0f){

    switch (m_category)
    {
    case CardCategory::ATTACK:
        m_cardBackground = Sprite(0.0f, 0.0f, 200.0f, 250.0f, Resources::Textures::CardBackground, FloatRect({ 0.05288f, 0.0f }, { 0.34375f, 1.f }), 1.0f);
         break;
    
    case CardCategory::DEFENCE:
        m_cardBackground = Sprite(0.0f, 0.0f, 200.0f, 250.0f, Resources::Textures::CardBackground, FloatRect({ 0.355352f, 0.0f }, { 0.636217f, 1.f }), 1.0f);
        break;

    case CardCategory::UTILITY:
        m_cardBackground = Sprite(0.0f, 0.0f, 200.0f, 250.0f, Resources::Textures::CardBackground, FloatRect({ 0.647820f, 0.0f }, { 0.933493f, 1.f }), 1.0f);
        break;
    }

    parseText();
}

Card::~Card() 
{
    m_text.clear();
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

//sets the position of the card icon in pixels
void Logic::Card::setIconPos(float x, float y, float width, float height)
{
    m_icon.setScreenPos(Sprite::TOP_LEFT, Sprite::TOP_LEFT, x, y, width, height);
}

//sets the position of the card background in pixels
void Logic::Card::setbackgroundPos(float x, float y, float width, float height)
{
    m_cardBackground.setScreenPos(Sprite::TOP_LEFT, Sprite::TOP_LEFT, x, y, width, height);
}

void Logic::Card::setCardTextPos(float posX)
{

    for (size_t i = 0; i < m_text.size(); i++)
    {
        m_text.at(i).position.x += posX;
    }

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

void Logic::Card::render() const
{
    m_cardBackground.render();
    m_icon.render();

    for(auto txt: m_text)
    {
        QueueRender(txt);
    }
}

//splits the text into smaler lines so they fit the cards
void Logic::Card::parseText()
{
    //title
    TextRenderInfo temp;
    temp.color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
    temp.font = Resources::Fonts::KG14;
    temp.position = DirectX::SimpleMath::Vector2(50.0f, 235.0f);
    temp.text = std::wstring(m_name.begin(), m_name.end());

    m_text.push_back(TextRenderInfo(temp));


    //description
    std::string splitString = m_description;

    int pos = 0;
   /* do
    {
        pos = splitString.find(' ', 12);
        splitString = splitString.substr(pos);

    }while (splitString.length() > 12);*/

    pos = splitString.find(' ', 14);
    //splitString = splitString.substr(pos);

    std::string endPart = m_description.substr(pos);
    splitString = m_description.substr(0, pos);

    //desc part 1
    temp.text = std::wstring(splitString.begin(), splitString.end());
    temp.position = DirectX::SimpleMath::Vector2(8.0f, 355.0f);
    m_text.push_back(TextRenderInfo(temp));

    //desc part 2
    temp.text = std::wstring(endPart.begin(), endPart.end());
    temp.position = DirectX::SimpleMath::Vector2(4.0f, 375.0f);
    m_text.push_back(TextRenderInfo(temp));
    
}
