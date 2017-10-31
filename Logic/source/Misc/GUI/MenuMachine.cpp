#include <Misc\GUI\MenuMachine.h>
#include <iostream>
#include <Misc\FileLoader.h>

#include <Keyboard.h>
#include <Engine\Typing.h>

#include <Engine\Profiler.h>

#define TRANSITION_TIME 400

using namespace Logic;

MenuMachine::MenuMachine()
{
	m_pressed = false;
	m_currentActiveMenu = nullptr;
	m_currentActiveState = gameStateMenuMain;
	m_highScoreNamePTR = nullptr;
	m_highScoreName = "";
	m_typing = false;
	m_forward = true;
    blinkMarker = true;
    blinkTimer = 0.0f;
    deleteCharCD = 20.0f;
	m_cardUpgrade = -1;
}

MenuMachine::MenuMachine(std::string* highScoreNamePTR)
{
	m_pressed = false;
	m_currentActiveMenu = nullptr;
	m_currentActiveState = gameStateMenuMain;
	m_highScoreNamePTR = highScoreNamePTR;
	m_highScoreName = *m_highScoreNamePTR;
	m_typing = false;
	m_forward = true;
    blinkTimer = 0.0f;
    deleteCharCD = 20.0f;
	m_cardUpgrade = -1;
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
	functions["buttonClick0"] = std::bind(&MenuMachine::startGame, this);
	functions["buttonClick1"] = std::bind(&MenuMachine::startSettings, this);
	functions["buttonClick2"] = std::bind(&MenuMachine::startMainMenu, this);
	functions["buttonClick3"] = std::bind(&MenuMachine::quitGame, this);
	functions["buttonClick4"] = std::bind(&MenuMachine::writing, this);
	functions["buttonClick5"] = std::bind(&MenuMachine::chooseUpgrade1, this);
	functions["buttonClick6"] = std::bind(&MenuMachine::chooseUpgrade2, this);
	functions["buttonClick7"] = std::bind(&MenuMachine::chooseUpgrade3, this);
	functions["buttonClick8"] = std::bind(&MenuMachine::minusSense, this);
	functions["buttonClick9"] = std::bind(&MenuMachine::plusSense, this);
	functions["buttonClick10"] = std::bind(&MenuMachine::minusMaster, this);
	functions["buttonClick11"] = std::bind(&MenuMachine::plusMaster, this);
	functions["buttonClick12"] = std::bind(&MenuMachine::minusSFX, this);
	functions["buttonClick13"] = std::bind(&MenuMachine::plusSFX, this);
	functions["buttonClick14"] = std::bind(&MenuMachine::muteUnmute, this);
	functions["buttonClick15"] = std::bind(&MenuMachine::minusFOV, this);
	functions["buttonClick16"] = std::bind(&MenuMachine::plusFOV, this);
	functions["buttonClick17"] = std::bind(&MenuMachine::windowed, this);
	functions["buttonClick18"] = std::bind(&MenuMachine::showHighscore, this);

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
			for (int i = 1; i <= menu.ints.at("buttonAmount"); i++)
			{
				tempButton.push_back(allButtons.at(menu.strings.at("button" + std::to_string(i))));
			}

			//Create new Menus and send in the fitting information from Menu vector
			m_menuStates[GameState(menu.ints.at("State"))] = newd MenuState();
			m_menuStates.at(GameState(menu.ints.at("State")))->initialize(tempButton, menu.ints.at("Background"));
		}
	}

	m_stateToBe = gameStateDefault;

	showMenu(state);
}

void MenuMachine::clear() 
{
	m_currentActiveMenu = nullptr;
}

void MenuMachine::update(float dt)
{
	PROFILE_BEGIN("Menu");
	auto Mouse = DirectX::Mouse::Get().GetState();

	if (Mouse.leftButton && !m_pressed && !m_typing)
	{
		m_pressed = true;
		m_currentActiveMenu->updateOnPress(Mouse.x, Mouse.y);
	}
	else if (!Mouse.leftButton && m_pressed)
	{
		m_pressed = false;

	}
	m_currentActiveMenu->hoverOver(Mouse.x, Mouse.y);

	if (m_stateToBe != gameStateDefault && !m_typing)
	{
		if (m_forward)
		{
			if (m_currentActiveMenu->animationTransition(dt, TRANSITION_TIME, m_forward))
			{
				showMenu(m_stateToBe);
				m_forward = false;
			}
		}
		else
		{
			if (m_currentActiveMenu->animationTransition(dt, TRANSITION_TIME, m_forward))
			{
				m_stateToBe = gameStateDefault;
				m_forward = true;
			}
		}
	}

	if (m_typing)
	{
        if (blinkMarker == false)
        {
            blinkTimer += dt;
            if (blinkTimer > 500.0f)
            {
                blinkMarker = true;
                blinkTimer = 0.0f;
            }
        }
        else
        {
            blinkTimer += dt;
            if (blinkTimer > 500.0f)
            {
                blinkMarker = false;
                blinkTimer = 0.0f;
            }
        }

        
        deleteCharCD += dt;
		DirectX::Keyboard::State keyboard = DirectX::Keyboard::Get().GetState();
		Typing* theChar = Typing::getInstance(); //might need to be deleted
		char tempChar = theChar->getSymbol();
		if (keyboard.IsKeyDown(DirectX::Keyboard::Enter))
		{
			m_typing = false;
			*m_highScoreNamePTR = m_highScoreName;
		}
		else if (keyboard.IsKeyDown(DirectX::Keyboard::Back) && m_highScoreName != "" && deleteCharCD > 200.0f)
		{
            m_highScoreName.erase(m_highScoreName.length() - 1, 1);
            deleteCharCD = 0.0f;
		}
		else if (tempChar != 0 && tempChar >= 32 && tempChar <= 122)
		{
			m_highScoreName += tempChar;
		}
	}
	PROFILE_END();
}

void MenuMachine::render(Graphics::Renderer &renderer, std::string highScore[10])
{
	PROFILE_BEGIN("Menu Render");
    if (m_currentActiveState == gameStateMenuSettings)
    {
        std::wstring tempString = L"";
        Graphics::ButtonInfo tempButton = m_currentActiveMenu->getMenuInfo().m_buttons.at(0);
        DirectX::SimpleMath::Vector2 tempPos;
        tempPos.x = tempButton.m_rek.x +tempButton.m_rek.width;
        tempPos.y = tempButton.m_rek.y + tempButton.m_rek.height -50;
        tempString.assign(m_highScoreName.begin(), m_highScoreName.end());
        if (m_typing)
        {
            if (blinkMarker)
            {
                tempString += L"|";
            }

        }
        Graphics::TextString text
        {
            tempString.c_str(),
            tempPos,
            DirectX::SimpleMath::Color(DirectX::Colors::White),
            Graphics::Font::SMALL
        };
        renderer.queueText(&text);
    }
	if (m_currentActiveState == gameStateHighscore)
	{
		std::string tempString = "";
		std::wstring tempWString = L"";
		DirectX::SimpleMath::Vector2 tempPos;
		for (int i = 0; i < 10; i++)
		{
			if (highScore[i].compare("") == 0)
			{
				break;
			}
			else
			{
				tempString += highScore[i] + "\n";
			}
		}
		tempWString.assign(tempString.begin(), tempString.end());
		tempPos.x = 400.0f;
		tempPos.y = 300.0f;

		Graphics::TextString text
		{
			tempWString.c_str(),
			tempPos,
			DirectX::SimpleMath::Color(DirectX::Colors::White),
			Graphics::Font::SMALL
		};
		renderer.queueText(&text);
	}

	if (m_currentActiveState == gameStateGameOver)
	{
		std::string tempString = "";
		std::wstring tempWString = L"";
		DirectX::SimpleMath::Vector2 tempPos;
		for (int i = 0; i < 10; i++)
		{
			if (highScore[i].compare("") == 0)
			{
				break;
			}
			else
			{
				tempString += highScore[i] + "\n";
			}
		}
		tempWString.assign(tempString.begin(), tempString.end());
		tempPos.x = 400.0f;
		tempPos.y = 300.0f;

		Graphics::TextString text
		{
			tempWString.c_str(),
			tempPos,
			DirectX::SimpleMath::Color(DirectX::Colors::White),
			Graphics::Font::SMALL
		};
		renderer.queueText(&text);
	}
    

    Graphics::MenuInfo temp = this->m_currentActiveMenu->getMenuInfo();

    renderer.drawMenu(&temp);
	PROFILE_END();
}

//Switches the currentState used 
void MenuMachine::showMenu(GameState state)
{
	if (m_menuStates.find(state) != m_menuStates.end())
	{
		if (state != gameStateGame)
		{
			DirectX::Mouse::Get().SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		}
		else
		{

		}
		m_currentActiveMenu = m_menuStates.at(state);
		m_currentActiveState = state;
	}
	else
	{
		m_currentActiveMenu = m_menuStates.at(gameStateDefault); //change to error state
		m_currentActiveState = gameStateDefault;
	}
}

GameState MenuMachine::currentState()
{
	return m_currentActiveState;
}

//Sets the state that the game is gonna show after the animation cycle has finished
void MenuMachine::setStateToBe(GameState gameState)
{
	
	m_stateToBe = gameState;
}

//Gets the state that the game is gonna show after the animation cycle has finished
GameState MenuMachine::getStateToBe()
{

	return m_stateToBe;
}

int Logic::MenuMachine::getChoiceUpgrade()
{
	int choosenUpgrade = -1;
	if (m_cardUpgrade != -1)
	{
		choosenUpgrade = m_cardUpgrade;
		m_cardUpgrade = -1;
	}
	return choosenUpgrade;
}

void MenuMachine::startGame()
{
	m_stateToBe = gameStateGame;
}

void MenuMachine::startSettings()
{
	m_stateToBe = gameStateMenuSettings;
}

void MenuMachine::startMainMenu()
{
	m_stateToBe = gameStateMenuMain;
}

void MenuMachine::quitGame()
{
	PostQuitMessage(0); 
}

void MenuMachine::writing()
{
	//triggers the typing button
	Typing* theChar = Typing::getInstance(); //might need to be deleted
	char trashThis = theChar->getSymbol();
	m_typing = true;
	m_highScoreName = "";
}

void MenuMachine::chooseUpgrade1() //Upgrade button1
{
	m_cardUpgrade = choice1;
}

void MenuMachine::chooseUpgrade2() //Upgrade button2
{
	m_cardUpgrade = choice2;
}

void MenuMachine::chooseUpgrade3() //Upgrade button3
{
	m_cardUpgrade = choice3;
}
void MenuMachine::plusSense()
{

}
void MenuMachine::minusSense()
{

}

void MenuMachine::plusMaster()
{

}
void MenuMachine::minusMaster()
{

}

void Logic::MenuMachine::plusSFX()
{

}

void Logic::MenuMachine::minusSFX()
{

}

void Logic::MenuMachine::muteUnmute()
{

}

void Logic::MenuMachine::plusFOV()
{

}

void Logic::MenuMachine::minusFOV()
{

}
void Logic::MenuMachine::windowed()
{

}
void Logic::MenuMachine::showHighscore()
{
	m_stateToBe = gameStateHighscore;
}
