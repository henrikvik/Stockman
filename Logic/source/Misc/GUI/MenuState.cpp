#include <Misc\GUI\MenuState.h>
using namespace Logic;

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::initialize(std::vector<ButtonStruct> buttonStruct, int background)
{
	Button button;
	for (auto const& struc : buttonStruct)
	{
		DirectX::SimpleMath::Vector2 pos(struc.xPos, struc.yPos);
		DirectX::SimpleMath::Vector2 texCoordStart(struc.xTexStart, struc.yTexStart);
		DirectX::SimpleMath::Vector2 texCoordEnd(struc.xTexEnd, struc.yTexEnd);
		
		button.initialize(pos, texCoordStart, texCoordEnd, struc.activeOffset,
			struc.height, struc.width, struc.textureIndex, struc.m_CallBackFunction);
		m_buttons.push_back(button);
    }
    m_menu.m_menuTexture = background;
}

void MenuState::updateOnPress(int posX, int posY)
{
	for (Button &b : m_buttons)
	{
		b.updateOnPress(posX, posY);
	}
}

void MenuState::hoverOver(int posX, int posY)
{
    for (Button &b : m_buttons)
    {
        b.hoverOver(posX, posY);
    }
}

bool MenuState::animationTransition(float dt, float maxAnimationTime, bool forward)
{
	bool done = true;

	for (Button &b : m_buttons)
	{
		// i dont know how this is suppose to work, the last item in the list is very important
		done = b.animationTransition(dt, maxAnimationTime, forward);
	}

	return done;
}

Graphics::MenuInfo MenuState::getMenuInfo()
{
    m_menu.m_buttons.clear();
    for (Button &b : m_buttons)
    {
        m_menu.m_buttons.push_back(b.getButtonInfo());
    }
    return m_menu;
}

