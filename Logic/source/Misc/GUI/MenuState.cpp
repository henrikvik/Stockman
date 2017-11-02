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
    {2, Resources::Textures::SettingsMenuButtons},
    {3, Resources::Textures::Color_pick_icons},
    {4, Resources::Textures::Backbutton}
};

void MenuState::initialize(std::vector<ButtonStruct> buttonStruct, int background)
{
	for (auto const& struc : buttonStruct)
	{
        FloatRect screenRect = {
            struc.xPos   / (float)WIN_WIDTH,
            struc.yPos   / (float)WIN_HEIGHT,
            struc.width  / (float)WIN_WIDTH,
            struc.height / (float)WIN_HEIGHT
        };
        FloatRect inactive = {
            {struc.xTexStart, struc.yTexStart},
            {struc.xTexEnd,   struc.yTexEnd} 
        };
        FloatRect active = {
            {struc.xTexStart, struc.yTexStart + struc.activeOffset},
            {struc.xTexEnd,   struc.yTexEnd   + struc.activeOffset} 
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

void Logic::MenuState::render() const
{
    for (Button const& b : m_buttons)
    {
        b.render();
    }
}

