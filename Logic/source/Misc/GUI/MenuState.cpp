#include <Graphics\include\RenderQueue.h>

#include <Misc\GUI\MenuState.h>
using namespace Logic;

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

const std::map<int, Resources::Textures::Files> tempTextureLookup =
{
    {0, Resources::Textures::mainMenuButton},
    {1, Resources::Textures::gameOverMenuButtons},
    {2, Resources::Textures::SettingsMenuButtons}
};

void MenuState::initialize(std::vector<ButtonStruct> buttonStruct, int background)
{
	for (auto const& struc : buttonStruct)
	{
        DirectX::SimpleMath::Rectangle screenRect = {
            (long) struc.xPos, 
            (long) struc.yPos,
            (long) struc.width,
            (long) struc.height
        };
        DirectX::SimpleMath::Rectangle inactive = {
            (long) struc.xTexStart,
            (long) struc.yTexStart,
            (long) (struc.xTexEnd - struc.xTexStart),
            (long) (struc.yTexEnd - struc.yTexStart)
        };
        DirectX::SimpleMath::Rectangle active = {
            (long) struc.xTexStart,
            (long) (struc.yTexStart + struc.activeOffset),
            (long) (struc.xTexEnd - struc.xTexStart),
            (long) (struc.yTexEnd - struc.yTexStart)
        };

        Resources::Textures::Files texture = tempTextureLookup.at(struc.texture);

        m_buttons.emplace_back(
            texture,
            screenRect,
            inactive, 
            active,
            struc.m_CallBackFunction
        );
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

	//for (Button &b : m_buttons)
	//{
	//	// i dont know how this is suppose to work, the last item in the list is very important
	//	done = b.animationTransition(dt, maxAnimationTime, forward);
	//}

	return done;
}

Button* MenuState::getButton(int index)
{
    return &m_buttons[index];
}

Graphics::MenuInfo MenuState::getMenuInfo()
{
    m_menu.m_buttons.clear();
    for (Button &b : m_buttons)
    {
        m_menu.m_buttons.push_back(*b.getButtonInfo());
    }
    return m_menu;
}

void Logic::MenuState::render() const
{
    for (Button const& b : m_buttons)
    {
        b.render();
    }
}

