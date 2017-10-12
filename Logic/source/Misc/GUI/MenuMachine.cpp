#include <Misc\GUI\MenuMachine.h>
#include <iostream>
#include <Misc\FileLoader.h>

#define TRANSITION_TIME 350.f

using namespace Logic;

MenuMachine::MenuMachine()
{
	pressed = false;
	currentActiveMenu = nullptr;
	currentActiveState = gameStateMenuMain;
}


MenuMachine::~MenuMachine()
{
	for (const auto& a : m_menuStates)
	{
		delete a.second;
	}
}

void MenuMachine::initialize(GameState state)
{
	//Gather all the functions in a map for future allocation
	std::map<std::string, std::function<void(void)>> functions;
	functions["buttonClick0"] = std::bind(&MenuMachine::buttonClick0, this);
	functions["buttonClick1"] = std::bind(&MenuMachine::buttonClick1, this);
	functions["buttonClick2"] = std::bind(&MenuMachine::buttonClick2, this);
	functions["buttonClick3"] = std::bind(&MenuMachine::buttonClick3, this);

	//Load the lw file information
	std::vector<FileLoader::LoadedStruct> buttonFile;
	FileLoader::singleton().loadStructsFromFile(buttonFile, "Button");
	std::vector<FileLoader::LoadedStruct> menuFile;
	FileLoader::singleton().loadStructsFromFile(menuFile, "Menu");

	//Gather all the buttons in a map for future allocation
	std::map<std::string, MenuState::ButtonStruct> allButtons;

	for (auto const& button : buttonFile)
	{
		//If it is a button add it into its map
		if (button.strings.find("buttonName") != button.strings.end())
		{
			std::string functionName = button.strings.at("function");
			allButtons[button.strings.at("buttonName")] = MenuState::ButtonStruct({
				button.floats.at("xPos"),
				button.floats.at("yPos"),
				button.floats.at("xTexStart"),
				button.floats.at("yTexStart"),
				button.floats.at("xTexEnd"),
				button.floats.at("yTexEnd"),
                button.floats.at("activeOffset"),
				button.floats.at("height"),
				button.floats.at("width"),
				button.ints.at("texture"),
				functions.at(functionName)
			});
		}
	}

	//Gather all the Menus in this vector for fututre use
	for (auto const& menu : menuFile)
	{
		//If it is a menu add one to the vector
		if (menu.ints.find("State") != menu.ints.end())
		{
			//Temporary Button Vector until Menu has been given them
			std::vector<MenuState::ButtonStruct> tempButton;
			for (int i = 0; i < menu.ints.at("buttonAmount"); i++)
			{
				tempButton.push_back(allButtons.at(menu.strings.at("button" + std::to_string(i + 1))));
			}

			//Create new Menus and send in the fitting information from Menu vector
			m_menuStates[GameState(menu.ints.at("State"))] = newd MenuState();
			m_menuStates.at(GameState(menu.ints.at("State")))->initialize(tempButton, menu.ints.at("Background"));
		}
	}

	stateToBe = gameStateDefault;

	showMenu(state);
}

void MenuMachine::clear() 
{
	currentActiveMenu = nullptr;
}

void MenuMachine::update(float dt)
{
	DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	auto Mouse = DirectX::Mouse::Get().GetState();

	if (Mouse.leftButton && !pressed)
	{
		pressed = true;
		currentActiveMenu->updateOnPress(Mouse.x, Mouse.y);
	}
	else if (!Mouse.leftButton && pressed)
	{
		pressed = false;

	}
    currentActiveMenu->hoverOver(Mouse.x, Mouse.y);

	if (stateToBe != gameStateDefault)
	{
		if (forward)
		{
			if (currentActiveMenu->animationTransition(dt, TRANSITION_TIME, forward))
			{
				showMenu(stateToBe);
				forward = false;
			}
		}
		else
		{
			if (currentActiveMenu->animationTransition(dt, TRANSITION_TIME, forward))
			{
				stateToBe = gameStateDefault;
				forward = true;
			}
		}
	}

}

void MenuMachine::render(Graphics::Renderer & renderer)
{
    Graphics::MenuInfo temp = this->currentActiveMenu->getMenuInfo();

    renderer.drawMenu(&temp);

}

//Switches the currentState used 
void MenuMachine::showMenu(GameState state)
{
	if (m_menuStates.find(state) != m_menuStates.end())
	{
		currentActiveMenu = m_menuStates.at(state);
		currentActiveState = state;
	}
	else
	{
		currentActiveMenu = m_menuStates.at(gameStateDefault); //change to error state
		currentActiveState = gameStateDefault;
	}
}

GameState MenuMachine::currentState()
{
	return currentActiveState;
}

//Sets the state that the game is gonna show after the animation cycle has finished
void MenuMachine::setStateToBe(GameState gameState)
{
	
	stateToBe = gameState;
}

//Gets the state that the game is gonna show after the animation cycle has finished
GameState MenuMachine::getStateToBe()
{

	return stateToBe;
}

void MenuMachine::buttonClick0()
{
	stateToBe = gameStateGame;
}

void MenuMachine::buttonClick1()
{
	stateToBe = gameStateMenuSettings;
}

void MenuMachine::buttonClick2()
{
	stateToBe = gameStateMenuMain;
}

void MenuMachine::buttonClick3()
{
	PostQuitMessage(0); 
}
